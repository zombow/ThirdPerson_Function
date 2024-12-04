// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "TPS_AbilitySet.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_AbilitySet : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Abilities")
	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> TPSAbilities;

public:
	TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> GetTPSAbilities();
};
