// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPS_InputConfig.h"
#include "GameFramework/PlayerController.h"
#include "TPS_PlayerController.generated.h"

/**
 * 
 */

UCLASS()

class THIRDPERSON_FUNCTION_API ATPS_PlayerController : public APlayerController
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
	TObjectPtr<UTPS_InputConfig> InputConfig;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInputs, FInputActionInstance, Instance, FGameplayTag, Value);
	FInputs OnInputs; // InputInstance와 FGameplayTag를 전달하는 델리게이트
	void HandleInputs(const FInputActionInstance& Instance, FGameplayTag Tag);

	void BindInputDataSet(TMap<FGameplayTag, TObjectPtr<UInputAction>>* InputActionTagMap);
};
