// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "TPS_GameplayEffect_StaminaRegen.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_GameplayEffect_StaminaRegen : public UGameplayEffect
{
	GENERATED_BODY()
	UTPS_GameplayEffect_StaminaRegen();

public:
	void SetModifier(float DeltaSeconds, float ScaleFloat);
};
