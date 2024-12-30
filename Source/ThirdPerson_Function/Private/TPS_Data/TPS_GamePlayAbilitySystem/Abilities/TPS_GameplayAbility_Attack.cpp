#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Attack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_AttackCost.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_DrawWeapon.h"

class UAbilityTask_PlayMontageAndWait;

UTPS_GameplayAbility_Attack::UTPS_GameplayAbility_Attack()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack")));
	CostGameplayEffectClass = UTPS_GameplayEffect_AttackCost::StaticClass();
	bNextAttack = false;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTPS_GameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player && !bNextAttack)
	{
		bNextAttack = true;
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Player->GetMesh()->GetAnimInstance());

		AttackEventHandle = Player->GetAbilitySystemComponent()->AddGameplayEventTagContainerDelegate(
			FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Attack"))),
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UTPS_GameplayAbility_Attack::Attack));

		if (!Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Drawn")))
		{
			FGameplayAbilitySpec* Target = Player->GetAbilitySpec(FGameplayTag::RequestGameplayTag("Ability.DrawWeapon"));
			if (Cast<UTPS_GameplayAbility_DrawWeapon>(Target->Ability))
			{
				Player->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
				DrawEventHandle = Player->GetAbilitySystemComponent()->AddGameplayEventTagContainerDelegate(
					FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Drawn"))),
					FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UTPS_GameplayAbility_Attack::DrawEndHandle));
				Player->GetAbilitySystemComponent()->TryActivateAbility(Target->Handle);
			}
		}
		else
		{
			Attack(FGameplayTag::RequestGameplayTag("State.Character.Attack.Combo1"));
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
		Player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		Player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
		UE_LOG(LogTemp, Warning, TEXT("End Attack"));
	}
}

void UTPS_GameplayAbility_Attack::Attack(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) && bNextAttack)
	{
		bNextAttack = false;
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
		Player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		Player->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));

		PlayMontage(EventTag);
	}
}

void UTPS_GameplayAbility_Attack::PlayMontage(const FGameplayTag SectionName)
{
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontage,
		1.0f,
		*SectionName.ToString(),
		true
	);
	Task->OnCompleted.AddDynamic(this, &UTPS_GameplayAbility_Attack::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_Attack::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_Attack::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_Attack::OnMontageCompleted()
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
