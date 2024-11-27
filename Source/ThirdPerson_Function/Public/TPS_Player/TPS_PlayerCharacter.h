// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "TPS_InputConfig.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPS_Player/TPS_CharacterMovementComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
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
	// Components
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "SkeletalMeshComp")
	TObjectPtr<USkeletalMeshComponent> PlayerSkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "CameraCompo")
	TObjectPtr<UCameraComponent> PlayerCameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "CameraBoom")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MoveComp")
	TObjectPtr<UTPS_CharacterMovementComponent> TPSCharacterMoveComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GAS")
	TObjectPtr<UTPS_AbilitySystemComponent> TPSAbilitySystemComp;
	
	UFUNCTION()
	void Move(FVector2D Value);
	UFUNCTION()
	void DoJump();
	UFUNCTION()
	void Crouching();
	UFUNCTION()
	void UnCrouching();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
