// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerCameraManager.h"

#include "TPS_Player/TPS_PlayerController.h"

void ATPS_PlayerCameraManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<ATPS_PlayerCharacter>(GetOwningPlayerController()->GetPawn());
	Camera = PlayerCharacter->FindComponentByClass<UCameraComponent>();
	CameraBoom = PlayerCharacter->FindComponentByClass<USpringArmComponent>();

	if (PlayerCharacter)
	{
		
		// 델리게이트 바인딩
		Cast<ATPS_PlayerController>(GetOwningPlayerController())->OnMouseMoveInput.AddDynamic(this, &ATPS_PlayerCameraManager::HandleLookInput);
	}
}

void ATPS_PlayerCameraManager::HandleLookInput(FVector2D Value)
{
	FVector2D LookAxisValue = Value;
	float LookSensitivity = 1.0f; // TODO:민감도 설정 (옵션 설정으로 이동필요)
	FRotator NewRotation = CameraBoom->GetRelativeRotation();

	NewRotation.Yaw += (LookAxisValue.X * LookSensitivity);
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + (LookAxisValue.Y * LookSensitivity), -88.0f, 88.0f);
	
	CameraBoom->SetRelativeRotation(NewRotation);
}
