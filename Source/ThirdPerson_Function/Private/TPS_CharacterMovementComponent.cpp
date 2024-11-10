// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_CharacterMovementComponent.h"


void UTPS_CharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode,
                                                            uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	if (MovementMode == MOVE_Falling)
	{
		StarFlyingTimer();
	}
	else if (PreviousMovementMode == MOVE_Falling)
	{
		StopFlyingTimer();
	}
}

void UTPS_CharacterMovementComponent::SetFlyingTime()
{
	FlyingTime += GetWorld()->DeltaTimeSeconds;
}

void UTPS_CharacterMovementComponent::StarFlyingTimer()
{
	FlyingTime = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(FlyingTimerHandle, this, &UTPS_CharacterMovementComponent::SetFlyingTime,
	                                       GetWorld()->DeltaTimeSeconds, true);
}

void UTPS_CharacterMovementComponent::StopFlyingTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(FlyingTimerHandle);
}
