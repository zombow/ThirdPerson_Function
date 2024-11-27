// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TPS_PlayerCharacter.h"
#include "TPS_PlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API ATPS_PlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Player")
	TObjectPtr<ATPS_PlayerCharacter> PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleLookInput(FVector2D Value);
};
