// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TPS_InputConfig.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_InputConfig : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
	TMap<FGameplayTag, TObjectPtr<UInputAction>> InputActionTagMap;

public:
	UInputMappingContext* GetImc() { return InputMappingContext; }
	TMap<FGameplayTag, TObjectPtr<UInputAction>>* GetInputActionTagMap() { return &InputActionTagMap; }
};
