// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_SheathWeapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPS_Animation/TPS_AnimInstance.h"
#include "TPS_Player/TPS_PlayerCharacter.h"

UTPS_GameplayAbility_SheathWeapon::UTPS_GameplayAbility_SheathWeapon()
{
	AbilityTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Ability.SheathWeapon"));
}

void UTPS_GameplayAbility_SheathWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		PlayMontage();
	}
}

void UTPS_GameplayAbility_SheathWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		Cast<UTPS_AnimInstance>(Cast<ACharacter>(CurrentActorInfo->AvatarActor)->GetMesh()->GetAnimInstance())->bisPlayingMontage = false;
		player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
	}
	SheathIn.Broadcast();
}

void UTPS_GameplayAbility_SheathWeapon::PlayMontage()
{
	Cast<UTPS_AnimInstance>(Cast<ACharacter>(CurrentActorInfo->AvatarActor)->GetMesh()->GetAnimInstance())->bisPlayingMontage = true;

	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		SheathMontage,
		1.0f,
		NAME_None,
		true
	);
	Task->OnCompleted.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_SheathWeapon::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_SheathWeapon::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_SheathWeapon::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
