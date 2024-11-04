// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_PlayerCameraManager.h"

void ATPS_PlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerController()->GetPawn());
	Camera = PlayerCharacter->FindComponentByClass<UCameraComponent>();
	CameraBoom = PlayerCharacter->FindComponentByClass<USpringArmComponent>();

	if (PlayerCharacter)
	{
		// 델리게이트 바인딩
		PlayerCharacter->OnLookInput.AddDynamic(this, &ATPS_PlayerCameraManager::HandleLookInput);
	}
}

void ATPS_PlayerCameraManager::HandleLookInput(FVector3d Value)
{
	FVector3d LookAxisValue = Value;
	float LookSensitivity = 1.0f; // TODO:민감도 설정 (옵션 설정으로 이동필요)
	// Yaw 회전 (좌우)
	FRotator NewRotation = CameraBoom->GetRelativeRotation();
	NewRotation.Yaw += (LookAxisValue.X * LookSensitivity);  
	// Pitch 회전 (상하)
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + (LookAxisValue.Y * LookSensitivity), -88.0f, 88.0f); // Pitch 범위를 제한
	// 	
	CameraBoom->SetRelativeRotation(NewRotation);
}
