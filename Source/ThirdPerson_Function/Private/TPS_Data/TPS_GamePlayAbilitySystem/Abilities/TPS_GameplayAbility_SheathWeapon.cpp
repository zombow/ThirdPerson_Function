// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_SheathWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UTPS_GameplayAbility_SheathWeapon::UTPS_GameplayAbility_SheathWeapon()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.SheathWeapon"));
}


void UTPS_GameplayAbility_SheathWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Target = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Target)
	{
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Target->GetMesh()->GetAnimInstance());
		if (ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target))
		{
			PlayMontage();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPS_GameplayAbility_SheathWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                   const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (Target)
	{
		PlayerAnimInstance->bisPlayingMontage = false;
	}
}

void UTPS_GameplayAbility_SheathWeapon::PlayMontage()
{
	if (Target)
	{
		PlayerAnimInstance->bisPlayingMontage = true;
	}

	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		SheathMontage,
		1.0f,
		SheathMontage->CompositeSections[0].SectionName,
		true
	);
	Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::KeepPlayMontage);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_SheathWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_SheathWeapon::KeepPlayMontage()
{
	ASC->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
	
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		SheathMontage,
		1.0f,
		SheathMontage->CompositeSections[1].SectionName,
		true
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
