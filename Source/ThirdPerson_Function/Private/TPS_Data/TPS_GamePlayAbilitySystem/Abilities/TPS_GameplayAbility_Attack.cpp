#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_AttackCost.h"
#include "TPS_Interface/AbilityResourceInterface.h"

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
	Target = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Target)
	{
		ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Target->GetMesh()->GetAnimInstance());

		// 콤보공격을 활성화시킬 Event
		AbilityEventMake(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Attack")), &UTPS_GameplayAbility_Attack::BeginNextAttackState,
		                 AttackEventHandle);
		AbilityEventMake(FGameplayTag::RequestGameplayTag(TEXT("State.Character.EndAttack")), &UTPS_GameplayAbility_Attack::EndNextAttackState,
		                 EndAttackEventHandle);

		// 납도상태에서 공격을 시도했다면 Draw부터 실행
		if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Drawn")))
		{
			FGameplayTagContainer DrawTagContainer = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Ability.DrawWeapon"));
			TArray<FGameplayAbilitySpec*> TargetAbilities;
			ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(DrawTagContainer, TargetAbilities);

			if (IsValid(TargetAbilities[0]->Ability))
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
				// Draw가 끝나면 공격을하는 Event
				AbilityEventMake(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")), &UTPS_GameplayAbility_Attack::DrawEndHandle,
				                 DrawEventHandle);
				ASC->TryActivateAbility(TargetAbilities[0]->Handle); // 여기서 Draw를 실행
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

	if (Target)
	{
		IAbilityResourceInterface::Execute_StartAbilityResource(Target);
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
		
		// Handles 일괄삭제
		for (TPair<FGameplayTag, FDelegateHandle> TargetHandle : GameplayEventHandles)
		{
			//ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(TargetHandle.Key), TargetHandle.Value);
		}
	}
}


void UTPS_GameplayAbility_Attack::Attack()
{
	if (bNextAttack && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		PlayMontage(NextSectionTag);
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
	}
	else if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		PlayMontage(FGameplayTag::RequestGameplayTag("State.Character.Attack.Combo1"));
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack"));
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
	}
}

void UTPS_GameplayAbility_Attack::BeginNextAttackState(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	bNextAttack = true;
	NextSectionTag = EventTag;
	ASC->RemoveGameplayEventTagContainerDelegate(
		FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Attack"))), AttackEventHandle);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_Attack::EndNextAttackState(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	bNextAttack = false;
	ASC->RemoveGameplayEventTagContainerDelegate(
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

template <typename T>
void UTPS_GameplayAbility_Attack::AbilityEventMake(FGameplayTag EventTag, void (T::*Func)(const FGameplayTag,
                                                                                          const FGameplayEventData*),
                                                   FDelegateHandle& OutHandle)
{
	OutHandle = ASC->AddGameplayEventTagContainerDelegate(
		FGameplayTagContainer(EventTag),
		FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, Func));

	GameplayEventHandles.Add(FGameplayTag(EventTag), OutHandle);
}


void UTPS_GameplayAbility_Attack::DrawEndHandle(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	Attack();
	ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("State.Character.Drawn"))),
	                                             DrawEventHandle);
}
