#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Attack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_AttackCost.h"
#include "TPS_Interface/AbilityResourceInterface.h"

class UAbilityTask_PlayMontageAndWait;

UTPS_GameplayAbility_Attack::UTPS_GameplayAbility_Attack()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack")));
	CostGameplayEffectClass = UTPS_GameplayEffect_AttackCost::StaticClass();
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UTPS_GameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	// Handles 일괄삭제
	for (const TPair<FGameplayTagContainer, FDelegateHandle> TargetHandle : GameplayEventHandles)
	{
		ASC->RemoveGameplayEventTagContainerDelegate(TargetHandle.Key, TargetHandle.Value);
	}
	GameplayEventHandles.Empty();
	bNextAttack = false;
	Target = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Target)
	{
		ASC = Cast<UTPS_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target));
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Target->GetMesh()->GetAnimInstance());


		// 콤보어택을 위한 이벤트 등록
		AbilityEventMake(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Attack")), &UTPS_GameplayAbility_Attack::BeginNextAttackState,
		                 AttackEventHandle);
		AbilityEventMake(FGameplayTag::RequestGameplayTag(TEXT("Input.Attack")), &UTPS_GameplayAbility_Attack::ComboAttack, ComboAttackHandle);
		// 납도상태에서 공격을 시도했다면 Draw부터 실행
		if (!ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn"))))
		{
			FGameplayTagContainer DrawTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")));
			TArray<FGameplayAbilitySpec*> TargetAbilities;
			ASC->GetActivatableGameplayAbilitySpecsByAllMatchingTags(DrawTagContainer, TargetAbilities);
			CharacterStateComponent = ASC->GetCharacterStateComponent();
			if (!TargetAbilities.IsEmpty() || CharacterStateComponent || CharacterStateComponent->GetWeaponState() == ECharacterWeaponState::Drawing)
			{
				CharacterStateComponent->SetWeaponState(ECharacterWeaponState::DrawAttack);
				AbilityEventMake(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")), &UTPS_GameplayAbility_Attack::DrawEndHandle,
				                 DrawEventHandle);
				if (!TargetAbilities.IsEmpty())
				{
					ASC->TryActivateAbility(TargetAbilities[0]->Handle); // 여기서 Draw를 실행
				}
			}
		}
		else
		{
			// 발도상태라면 최초 공격시작
			Attack(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Attack.Combo1")));
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
		bNextAttack = false;
		IAbilityResourceInterface::Execute_StartAbilityResource(Target);
		ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Attack")));
	}
}


void UTPS_GameplayAbility_Attack::Attack(FGameplayTag AttackSection)
{
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		PlayMontage(AttackSection);
		ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Attack")));
	}
}

void UTPS_GameplayAbility_Attack::BeginNextAttackState(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	bNextAttack = true;
	NextSectionTag = EventTag;
}


void UTPS_GameplayAbility_Attack::ComboAttack(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	if (bNextAttack && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		PlayerAnimInstance->Montage_JumpToSection(NextSectionTag.GetTagName(), PlayerAnimInstance->GetCurrentActiveMontage());
		bNextAttack = false;
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
	FGameplayTagContainer TagContainer(EventTag);
	OutHandle = ASC->AddGameplayEventTagContainerDelegate(
		TagContainer,
		FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, Func));

	GameplayEventHandles.Add(FGameplayTag(EventTag), OutHandle);
}


void UTPS_GameplayAbility_Attack::DrawEndHandle(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	Attack(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Attack.Combo1")));
	ASC->RemoveGameplayEventTagContainerDelegate(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn"))),
	                                             DrawEventHandle);
}
