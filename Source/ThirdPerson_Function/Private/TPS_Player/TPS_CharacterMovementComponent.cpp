// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_CharacterMovementComponent.h"

#include "VectorTypes.h"


void UTPS_CharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode,
                                                            uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	MovementModeChange.Broadcast(PreviousMovementMode, MovementMode, PreviousCustomMode);
}

