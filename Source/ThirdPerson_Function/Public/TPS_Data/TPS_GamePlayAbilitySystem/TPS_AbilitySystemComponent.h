// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TPS_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
};
