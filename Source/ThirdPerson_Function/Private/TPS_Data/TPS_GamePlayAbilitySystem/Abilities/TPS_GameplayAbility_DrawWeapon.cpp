// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_DrawWeapon.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPS_Animation/TPS_AnimInstance.h"
#include "TPS_Player/TPS_PlayerCharacter.h"

UTPS_GameplayAbility_DrawWeapon::UTPS_GameplayAbility_DrawWeapon()
{
	AbilityTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")));
}

void UTPS_GameplayAbility_DrawWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
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
	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		player->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
		Cast<UTPS_AnimInstance>(Cast<ACharacter>(CurrentActorInfo->AvatarActor)->GetMesh()->GetAnimInstance())->bisPlayingMontage = false;
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn"));
		EventData.Instigator = player;
		player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void UTPS_GameplayAbility_DrawWeapon::PlayMontage()
{
	UTPS_AnimInstance* PlayerAnimInstance = Cast<UTPS_AnimInstance>(Cast<ACharacter>(CurrentActorInfo->AvatarActor)->GetMesh()->GetAnimInstance());
	PlayerAnimInstance->bisPlayingMontage = true;
	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DrawMontage,
		1.0f,
		"Draw1",
		true
	);
	PlayerAnimInstance->DrawCheck.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::JumpToSection);
	bKeepDraw = true;
	if (bKeepDraw)
		Task->OnCompleted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::KeepPlayMontage);
	else
		Task->OnCompleted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_DrawWeapon::KeepPlayMontage()
{
	UTPS_AnimInstance* PlayerAnimInstance = Cast<UTPS_AnimInstance>(Cast<ACharacter>(CurrentActorInfo->AvatarActor)->GetMesh()->GetAnimInstance());

	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DrawMontage,
		1.0f,
		"Draw2",
		true
	);

	Task->OnCompleted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCompleted);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
}

void UTPS_GameplayAbility_DrawWeapon::OnMontageCompleted()
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

void UTPS_GameplayAbility_DrawWeapon::JumpToSection()
{
	UTPS_AnimInstance* PlayerAnimInstance = Cast<UTPS_AnimInstance>(Cast<ACharacter>(CurrentActorInfo->AvatarActor)->GetMesh()->GetAnimInstance());
	PlayerAnimInstance->Montage_JumpToSection("Draw2", DrawMontage);
}
