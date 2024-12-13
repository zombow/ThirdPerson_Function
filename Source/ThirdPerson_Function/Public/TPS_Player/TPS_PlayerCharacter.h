// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "TPS_InputConfig.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPS_Player/TPS_CharacterMovementComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySet.h"
#include "TPS_PlayerCharacter.generated.h"

UCLASS()
class ATPS_PlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
protected:
	ATPS_PlayerCharacter(const FObjectInitializer& ObjectInitializer);
	// Sets default values for this character's properties
	virtual void PossessedBy(AController* NewController) override;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GAS")
	TObjectPtr<UTPS_AbilitySet> TPSAbilities;

	UFUNCTION()
	void MovementModeChanged(EMovementMode PreviousMovementMode, EMovementMode CurrentMovementMode, uint8 PreviousCustomMode);
	UFUNCTION()
	void Move(FVector2D Value);
	UFUNCTION()
	void DoJump();
	void EndJump();
	UFUNCTION()
	void Crouching();
	UFUNCTION()
	void UnCrouching();
	UFUNCTION()
	void DoRoll();
	UFUNCTION()
	void Attack();
	UFUNCTION()
	void DrawWeapon();

public:
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void AddLooseGameplayTag(FGameplayTag TagName);
	void RemoveLooseGameplayTag(FGameplayTag TagName);
	TObjectPtr<UTPS_CharacterMovementComponent> GetTPSCharacterMovementComp() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "SkeletalMeshComp")
	FVector2D InputDirection;
};
