// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_DrawWeapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UTPS_GameplayAbility_DrawWeapon::UTPS_GameplayAbility_DrawWeapon()
{
	AbilityTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")));
}


void UTPS_GameplayAbility_DrawWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player)
	{
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Player->GetMesh()->GetAnimInstance());
		PlayMontage();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPS_GameplayAbility_DrawWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (Player)
	{
		PlayerAnimInstance->bisPlayingMontage = false;
		Player->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
		
		FGameplayEventData EventData; // Draw종료 Event생성
		EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn"));
		EventData.Instigator = Player;
		Player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void UTPS_GameplayAbility_DrawWeapon::PlayMontage()
{
	if (Player)
	{
		PlayerAnimInstance->bisPlayingMontage = true;
	}
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DrawMontage,
		1.0f,
		"Draw1",
		true
	);

	if (Player->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack")))
		Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageBlendOut);
	else
		Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::KeepPlayMontage);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_DrawWeapon::KeepPlayMontage()
{
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DrawMontage,
		1.0f,
		"Draw2",
		true
	);

	Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageBlendOut);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_DrawWeapon::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
