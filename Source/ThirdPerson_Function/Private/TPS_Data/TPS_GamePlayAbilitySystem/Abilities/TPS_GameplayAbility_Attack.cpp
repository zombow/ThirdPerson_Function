#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Attack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_AttackCost.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_DrawWeapon.h"

class UAbilityTask_PlayMontageAndWait;

UTPS_GameplayAbility_Attack::UTPS_GameplayAbility_Attack()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack")));
	CostGameplayEffectClass = UTPS_GameplayEffect_AttackCost::StaticClass();
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTPS_GameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player)
	{
		Player->StaminaRegen(false);
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Player->GetMesh()->GetAnimInstance());
		// 콤보공격을 활성화시킬 Event
		AttackEventHandle = Player->GetAbilitySystemComponent()->AddGameplayEventTagContainerDelegate(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Attack"))),
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UTPS_GameplayAbility_Attack::BeginNextAttackState));
		EndAttackEventHandle = Player->GetAbilitySystemComponent()->AddGameplayEventTagContainerDelegate(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.EndAttack"))),
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UTPS_GameplayAbility_Attack::EndNextAttackState));
		// 납도상태에서 공격을 시도했다면 Draw부터 실행
		if (!Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Drawn")))
		{
			FGameplayAbilitySpec* Target = Cast<UTPS_AbilitySystemComponent>(Player->GetAbilitySystemComponent())->GetAbilitySpec
			(FGameplayTag::RequestGameplayTag("Ability.DrawWeapon"));
			if (Cast<UTPS_GameplayAbility_DrawWeapon>(Target->Ability))
			{
				Player->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
				// Draw가 끝나면 공격을하는 Event
				DrawEventHandle = Player->GetAbilitySystemComponent()->AddGameplayEventTagContainerDelegate(
					FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Drawn"))),
					FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UTPS_GameplayAbility_Attack::DrawEndHandle));
				Player->GetAbilitySystemComponent()->TryActivateAbility(Target->Handle);
			}
		}
		else
		{
			// 발도상태라면 최초 공격시작
			Attack();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPS_GameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);


	if (Player)
	{
		Player->StaminaRegen(true);
		Player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		Player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));

		UE_LOG(LogTemp, Error, TEXT("End Attack"));
	}
}


void UTPS_GameplayAbility_Attack::Attack()
{
	if (bNextAttack && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("ComboAttack!"));
		// 순간회전 -> 0.1초라도 회전시간추가
		auto InputVectorNormal = Player->GetActorForwardVector().GetSafeNormal();
		auto rotation = InputVectorNormal.Rotation();
		Player->SetActorRotation(FRotator(0, rotation.Yaw, 0));
		
		PlayMontage(NextSectionTag);
		Player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		Player->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
	}
	else if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
		// 순간회전 -> 0.1초라도 회전시간추가
		auto InputVectorNormal = Player->GetActorForwardVector().GetSafeNormal();
		auto rotation = InputVectorNormal.Rotation();
		Player->SetActorRotation(FRotator(0, rotation.Yaw, 0));
		
		PlayMontage(FGameplayTag::RequestGameplayTag("State.Character.Attack.Combo1"));
		Player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		Player->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
	}
}

void UTPS_GameplayAbility_Attack::BeginNextAttackState(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	bNextAttack = true;
	NextSectionTag = EventTag;
	Player->GetAbilitySystemComponent()->RemoveGameplayEventTagContainerDelegate(
		FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Attack"))), AttackEventHandle);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_Attack::EndNextAttackState(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	bNextAttack = false;
	Player->GetAbilitySystemComponent()->RemoveGameplayEventTagContainerDelegate(
		FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.EndAttack"))), EndAttackEventHandle);
}

void UTPS_GameplayAbility_Attack::PlayMontage(const FGameplayTag SectionName)
{
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontage,
		1.0f,
		*SectionName.ToString(),
		false
	);
	Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_Attack::OnMontageBlendOut);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_Attack::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_Attack::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_Attack::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_Attack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_Attack::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_Attack::DrawEndHandle(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	Attack();
	Player->GetAbilitySystemComponent()->RemoveGameplayEventTagContainerDelegate(
		FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Drawn"))), DrawEventHandle);
}
