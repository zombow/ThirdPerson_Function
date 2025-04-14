// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Interaction.h"

UTPS_GameplayAbility_Interaction::UTPS_GameplayAbility_Interaction()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Interaction")));
}

void UTPS_GameplayAbility_Interaction::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player)
	{
		// 플레이어가 인터렉션을 시도할경우 무엇을 할것인가?
		UE_LOG(LogTemp,Warning,TEXT("ON INTERACTION!!!"));
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UTPS_GameplayAbility_Interaction::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
