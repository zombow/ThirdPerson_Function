// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_CharacterMovementComponent.h"

#include "VectorTypes.h"


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
	CheckLandingRecovery();
}

void UTPS_CharacterMovementComponent::CheckLandingRecovery()
{
	float RecoveryTime = (FMath::Clamp(FlyingTime, MinFlyTime, MaxFlyTime) - MinFlyTime) / (MaxFlyTime - MinFlyTime);
	if (RecoveryTime >= LRThresholdTime)
	{
		// 착지반동 황성화
		bLandingRecovery = true;
		// 착지순간 체공시간을 뺀 속도를 디노멀라이즈된 상수로변경
		CurrentLandingWalkSpeed = (1 - RecoveryTime) * (CustomMaxWalkSpeed - MinLandingWalkSpeed) + MinLandingWalkSpeed;
		MaxWalkSpeed = CurrentLandingWalkSpeed;

		GetWorld()->GetTimerManager().SetTimer(LandingRecoveryTimerHandle, this,
		                                       &UTPS_CharacterMovementComponent::EndLandingRecovery,
		                                       GetWorld()->DeltaTimeSeconds, true);
	}
	else
	{
		bLandingRecovery = false;
	}
}

void UTPS_CharacterMovementComponent::EndLandingRecovery()
{
	// 체공시간을 노멀라이즈로 변환 (설정한 최대채공시간에 맞춰 노멀라이즈화)
	float RecoveryTime = (FMath::Clamp(FlyingTime, MinFlyTime, MaxFlyTime) - MinFlyTime) / (MaxFlyTime - MinFlyTime);
	// 반동시간을 최대1초로 기준하며, Lerp의 Alpha에 들어갈시간을 보강)
	float AlpTime = 1 / (RecoveryTime / GetWorld()->DeltaTimeSeconds);
	// 지속적으로 호출하여 static에 값보관
	static float CheckTime = AlpTime;
	CheckTime += AlpTime;
	UE_LOG(LogTemp, Warning, TEXT("%f / is CheckTime" ), CheckTime);
	// Lerp으로 최대이동속도를 자연스럽게 상승시킴
	MaxWalkSpeed = FMath::Lerp(CurrentLandingWalkSpeed, CustomMaxWalkSpeed, CheckTime);
	UE_LOG(LogTemp, Warning, TEXT("%f / isCurrent MaxSpeed" ), MaxWalkSpeed);
	if (MaxWalkSpeed >= 599.0f)
	{
		bLandingRecovery = false;
		MaxWalkSpeed = 600;
		CheckTime = 0;
		GetWorld()->GetTimerManager().ClearTimer(LandingRecoveryTimerHandle);
	}
}
