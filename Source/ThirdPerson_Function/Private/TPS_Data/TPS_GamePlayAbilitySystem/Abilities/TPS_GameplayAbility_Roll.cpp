// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Roll.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_RollCost.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UTPS_GameplayAbility_Roll::UTPS_GameplayAbility_Roll()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Roll")));
	CostGameplayEffectClass = UTPS_GameplayEffect_RollCost::StaticClass();
}

void UTPS_GameplayAbility_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player)
	{
		if (CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			auto InputVectorNormal = Player->GetLastMovementInputVector().GetSafeNormal();
			auto rotation = InputVectorNormal.Rotation();
			Player->SetActorRotation(FRotator(0, rotation.Yaw, 0));
			PlayMontage();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPS_GameplayAbility_Roll::PlayMontage()
{
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		RollMontage,
		1.0f,
		NAME_None,
		false
	);

	Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_Roll::OnMontageBlendOut);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_Roll::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_Roll::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_Roll::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_Roll::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_Roll::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
