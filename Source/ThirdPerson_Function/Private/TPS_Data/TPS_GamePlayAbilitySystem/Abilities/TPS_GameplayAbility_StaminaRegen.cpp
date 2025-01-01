// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_StaminaRegen.h"

#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_StaminaRegen.h"


UTPS_GameplayAbility_StaminaRegen::UTPS_GameplayAbility_StaminaRegen()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.StaminaRegen"));
}

void UTPS_GameplayAbility_StaminaRegen::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player)
	{
		FGameplayEffectSpecHandle StaminaEffectSpecHandle = MakeOutgoingGameplayEffectSpec(
			UTPS_GameplayEffect_StaminaRegen::StaticClass(), GetAbilityLevel());
		if (StaminaEffectSpecHandle.IsValid())
		{
			StaminaRegenHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, StaminaEffectSpecHandle);
			
		}
	}
}
