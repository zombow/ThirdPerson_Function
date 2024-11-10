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

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	void SetFlyingTime();
	void StarFlyingTimer();
	void StopFlyingTimer();
	FTimerHandle FlyingTimerHandle;
public:
	UFUNCTION(BlueprintCallable)
	float GetFlyingTime() { return FlyingTime; }
};
