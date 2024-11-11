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
	float FlyingTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "LandingRecovery")
	bool bLandingRecovery;
	
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	void SetFlyingTime();
	void StarFlyingTimer();
	void StopFlyingTimer();
	FTimerHandle FlyingTimerHandle;
	void CheckLandingRecovery();
	void EndLandingRecovery();
	FTimerHandle LandingRecoveryTimerHandle;
	float MinLandingWalkSpeed = 0.f;
	float CustomMaxWalkSpeed = 600.0f;
	float CurrentLandingWalkSpeed;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "LandingRecovery")
	float MaxFlyTime = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "LandingRecovery")
	float MinFlyTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin="0.0", ClampMax="1.0"), Category = "LandingRecovery")
	float LRThresholdTime = 0.5f;
	UFUNCTION(BlueprintCallable)
	float GetFlyingTime() { return FlyingTime; }
	UFUNCTION(BlueprintCallable)
	bool IsLandingRecovery() { return bLandingRecovery; }
};
