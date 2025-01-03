// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPS_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "FlyingTime")
	float FallingForce = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "LandingRecovery")
	bool bLandingRecovery;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	// FallingForce Check;
	void SetFallingForce();
	void CheckFallingForce(EMovementMode M_PreviousMovementMode);
	FTimerHandle FallingForceHandler;

	// LandingRecovery Check;
	void CheckLandingRecovery();
	void EndLandingRecovery();
	FTimerHandle LandingRecoveryTimerHandle;

	// CheckOptions
	float MaxRecoveryTime = 1.f;
	float MinLandingWalkSpeed = 0.f;
	float CheckAlphaTime = 0.f;
	float RecoveryTime = 0.f;
	float CurrentLandingWalkSpeed = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "LandingRecovery")
	float MaxLandingWalkSpeed = MaxWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "LandingRecovery")
	float MaxFallForce = -2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "LandingRecovery")
	float MinFallForce = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin="-2000.0", ClampMax="0.0"), Category = "LandingRecovery")
	float ThresholdForce = 0.5f;
	UFUNCTION(BlueprintCallable)
	float GetFallingForce() { return FallingForce; }

	UFUNCTION(BlueprintCallable)
	bool IsLandingRecovery() { return bLandingRecovery; }
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMovementModeChange,
	                                               EMovementMode, PreviousMovementMode,
	                                               EMovementMode, CurrentMovementMode,
	                                               uint8, PreviousCustomMode);
	FMovementModeChange MovementModeChange;
};
