// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TPS_InputConfig.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPS_CharacterMovementComponent.h"
#include "ThirdPerson_Function/ThirdPerson_FunctionCharacter.h"
#include "TPS_PlayerCharacter.generated.h"

UCLASS()
class ATPS_PlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPS_PlayerCharacter(const FObjectInitializer& ObjectInitializer);

	// 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLookInput, FVector3d, LookInput);

	FOnLookInput OnLookInput;

protected:
	// Componentes
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "SkeletalMeshComp")
	TObjectPtr<USkeletalMeshComponent> PlayerSkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "CameraCompo")
	TObjectPtr<UCameraComponent> PlayerCameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "CameraBoom")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MoveComp")
	TObjectPtr<UTPS_CharacterMovementComponent> TPSCharacterMoveComp;
	
	// Enhanced Bind
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Input")
	TObjectPtr<UTPS_InputConfig> InputConfig;
	
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void DoJump(const FInputActionValue& Value);
	void Crouching(const FInputActionValue& Value);
	void UnCrouching(const FInputActionValue& Value);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
