// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_SheathWeapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UTPS_GameplayAbility_SheathWeapon::UTPS_GameplayAbility_SheathWeapon()
{
	AbilityTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Ability.SheathWeapon"));
}


void UTPS_GameplayAbility_SheathWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player)
	{
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Player->GetMesh()->GetAnimInstance());
	}
	if (Player)
	{
		PlayMontage();
	}
}

void UTPS_GameplayAbility_SheathWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (Player)
	{
		PlayerAnimInstance->bisPlayingMontage = false;
		
	}
}

void UTPS_GameplayAbility_SheathWeapon::PlayMontage()
{
	if (Player)
	{
		PlayerAnimInstance->bisPlayingMontage = true;
	}
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		SheathMontage,
		1.0f,
		"Sheath1",
		true
	);
	Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::KeepPlayMontage);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_SheathWeapon::KeepPlayMontage()
{
	PlayerAnimInstance->bisPlayingMontage = true;
	if (Player)
	{
		Player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
	}
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		SheathMontage,
		1.0f,
		"Sheath2",
		false
	);
	Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageBlendOut);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}


void UTPS_GameplayAbility_SheathWeapon::OnMontageBlendOut()
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
