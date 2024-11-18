// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_CharacterMovementComponent.h"

#include "VectorTypes.h"


void UTPS_CharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode,
                                                            uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	CheckFallingForce(PreviousMovementMode);
}

void UTPS_CharacterMovementComponent::SetFallingForce()
{
	FallingForce = Velocity.Z;
}

void UTPS_CharacterMovementComponent::CheckFallingForce(EMovementMode M_PreviousMovementMode)
{
	if (MovementMode == MOVE_Falling)
	{
		GetWorld()->GetTimerManager().SetTimer(FallingForceHandler, this,
		                                       &UTPS_CharacterMovementComponent::SetFallingForce,
		                                       GetWorld()->DeltaTimeSeconds, MovementMode == MOVE_Falling);
	}
	else if (M_PreviousMovementMode == MOVE_Falling)
	{
		GetWorld()->GetTimerManager().ClearTimer(FallingForceHandler);
		CheckLandingRecovery();
	}
}

void UTPS_CharacterMovementComponent::CheckLandingRecovery()
{
	if (FallingForce <= ThresholdForce)
	{
		NavAgentProps.bCanJump = false;
		// FallingForce를 노멀라이즈 (0~1) 이후 최대낙하시간을 곱해 최종낙하시간 생성 
		RecoveryTime = ((FMath::Clamp(FallingForce, MaxFallForce, MinFallForce) - MinFallForce) / (MaxFallForce -
			MinFallForce)) * MaxRecoveryTime;
		bLandingRecovery = true;
		// RecoveryTime에 비례하여 착지순간의 속도를 감소 (MaxRecoveryTime이 1이상일경우 나누어줌)
		CurrentLandingWalkSpeed = ((MaxRecoveryTime - RecoveryTime) * (MaxLandingWalkSpeed - MinLandingWalkSpeed) /
			MaxRecoveryTime) + MinLandingWalkSpeed;
		MaxWalkSpeed = CurrentLandingWalkSpeed;

		GetWorld()->GetTimerManager().SetTimer(LandingRecoveryTimerHandle, this,
		                                       &UTPS_CharacterMovementComponent::EndLandingRecovery,
		                                       GetWorld()->DeltaTimeSeconds, true);
	}
	else
	{
		bLandingRecovery = false;
	}
	FallingForce = 0;
}

void UTPS_CharacterMovementComponent::EndLandingRecovery()
{
	// 알파는 1임으로 1로기준하여 Lerp의 Alpha에 들어갈시간을 보강 (RecoveringTime만큼 실행되도록)
	float AlpTime = 1 / (RecoveryTime / GetWorld()->DeltaTimeSeconds);
	CheckAlphaTime += AlpTime;
	// Lerp으로 최대이동속도를 자연스럽게 상승시킴 (1차그래프)
	MaxWalkSpeed = FMath::Lerp(CurrentLandingWalkSpeed, MaxLandingWalkSpeed, CheckAlphaTime);
	if (MaxWalkSpeed >= MaxLandingWalkSpeed - 1)
	{
		NavAgentProps.bCanJump = true;
		bLandingRecovery = false;
		FallingForce = 0;
		MaxWalkSpeed = MaxLandingWalkSpeed;
		CheckAlphaTime = 0;
		GetWorld()->GetTimerManager().ClearTimer(LandingRecoveryTimerHandle);
	}
}
