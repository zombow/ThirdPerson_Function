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
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;


public:
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMovementModeChange,
	                                               EMovementMode, PreviousMovementMode,
	                                               EMovementMode, CurrentMovementMode,
	                                               uint8, PreviousCustomMode);
	FMovementModeChange MovementModeChange;
};
