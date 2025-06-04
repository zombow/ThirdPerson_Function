// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySet.h"
#include "AbilitySystemComponent.h"
#include "TPS_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GAS")
	TMap<FGameplayTag, FGameplayAbilitySpec>TPSAbilitySpecs;
	
public:
	FGameplayAbilitySpec* GetAbilitySpecFromTag(FGameplayTag AbilityTag);
	void InitializeFromAbilitySet(UTPS_AbilitySet* AbilitySet);

};