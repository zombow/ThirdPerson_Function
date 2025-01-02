// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPS_Player/TPS_PlayerCharacter.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_StaminaRegen.h"
#include "TPS_GameplayAbility_StaminaRegen.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_GameplayAbility_StaminaRegen : public UGameplayAbility
{
	GENERATED_BODY()

	UTPS_GameplayAbility_StaminaRegen();
	UPROPERTY()
	TObjectPtr<ATPS_PlayerCharacter> Player;
	FActiveGameplayEffectHandle StaminaRegenHandle;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
