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
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMouseMoveInput, FVector2D, MouseMoveInput);
	FOnMouseMoveInput OnMouseMoveInput;
	void HandleMouseMoveInput(const FInputActionValue& Value);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveOngoing, FInputActionInstance, Instance);
	FOnMoveOngoing OnMoveOngoing;;
	void HandleMoveOngoing(const FInputActionInstance& Value);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveEnd, FInputActionInstance, Instance);
	FOnMoveEnd OnMoveEnd;;
	void HandleMoveEnd(const FInputActionInstance& Value);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotationInput, FVector2D, MoveInput);
	FOnRotationInput OnRotationInput;
	void HandleRotationInput(const FInputActionValue& Value);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotationRelease, FVector2D, MoveInput);
	FOnRotationRelease OnRotationRelease;
	void HandleRotationRelease(const FInputActionValue& Value);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpInput);
	FOnJumpInput OnJumpInput;
	void HandleJumpInput(const FInputActionValue& Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCrouching);
	FOnCrouching OnCrouching;
	void HandleOnCrouching(const FInputActionValue& Value);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnCrouching);
	FUnCrouching UnCrouching;
	void HandleUnCrouching(const FInputActionValue& Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRollInput);
	FOnRollInput OnRollInput;
	void HandleRollInput(const FInputActionValue& Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackInput);
	FAttackInput OnAttackInput;
	void HandleAttackInput(const FInputActionValue& Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDrawWeapon);
	FDrawWeapon OnDrawWeapon;
	void HandleDrawWeaponInput(const FInputActionValue& Value);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteraction);
	FInteraction OnInteraction;
	void HandleInteractionInput(const FInputActionValue& Value);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionHold);
	FInteractionHold OnInteractionHold;
	void HandleInteractionHold(const FInputActionValue& Value);
};
