// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityResourceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityResourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THIRDPERSON_FUNCTION_API IAbilityResourceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Resource")
	void StartAbilityResource();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Resource")
	void StopAbilityResource();
};
