// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputAction.h"
#include "TPS_InteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPS_Player/TPS_CharacterMovementComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Interface/AbilityResourceInterface.h"
#include "TPS_Props/TPS_InteractableActor.h"
#include "TPS_PlayerCharacter.generated.h"

UCLASS()
class ATPS_PlayerCharacter : public ACharacter, public IAbilitySystemInterface, public IAbilityResourceInterface
{
	GENERATED_BODY()

protected:
	ATPS_PlayerCharacter(const FObjectInitializer& ObjectInitializer);
	// Sets default values for this character's properties
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "SkeletalMeshComp")
	TObjectPtr<USkeletalMeshComponent> PlayerSkeletalMeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "CameraComp")
	TObjectPtr<UCameraComponent> PlayerCameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "CameraBoom")
	TObjectPtr<USpringArmComponent> CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "SceneComp")
	TObjectPtr<USceneComponent> WeaponSpot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MoveComp")
	TObjectPtr<UTPS_CharacterMovementComponent> TPSCharacterMoveComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GAS")
	TObjectPtr<UTPS_AbilitySystemComponent> TPSAbilitySystemComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "StateComp")
	TObjectPtr<UTPS_CharacterStateComponent> TPSCharacterStateComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TObjectPtr<UTPS_InteractionComponent> TPSInteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TObjectPtr<UBoxComponent> TargetInteractionBox;

	// direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MovementVelue")
	FVector DesiredDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "MovementVelue")
	FVector LandVelocity;
	
	
	TMap<FGameplayTag, TDelegate<void(const FInputActionInstance&)>> InputDelegates;
	UFUNCTION()
	void HandleInputs(FInputActionInstance Instance, FGameplayTag Tag); // receive from PlayerController
	UFUNCTION()
	void MovementModeChanged(EMovementMode PreviousMovementMode, EMovementMode CurrentMovementMode, uint8 PreviousCustomMode);
	UFUNCTION()
	void Look(const FInputActionInstance& Value);
	UFUNCTION()
	void Move(const FInputActionInstance& Value);

	// Ability Implementation
	UFUNCTION()
	void TryActivateAbilityByTag(FGameplayTag Tag); // helper function for activating abilities by tag
	UFUNCTION()
	void DoJump(const FInputActionInstance& Value);
	void EndJump(const FInputActionInstance& Value);
	virtual void Landed(const FHitResult& Hit) override;
	UFUNCTION()
	void Crouching(const FInputActionInstance& Value);
	UFUNCTION()
	void DoRoll(const FInputActionInstance& Value);
	UFUNCTION()
	void Attack(const FInputActionInstance& Value);
	UFUNCTION()
	void DrawAndSheathWeapon(const FInputActionInstance& Value);
	UFUNCTION()
	void Interaction(const FInputActionInstance& Value);

	// Interface
	void StartAbilityResource_Implementation() override;
	void StopAbilityResource_Implementation() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TSet<TObjectPtr<ATPS_InteractableActor>> InteractableActorArray;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TObjectPtr<ATPS_InteractableActor> FocusdInteractableActor;

	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void StaminaRegen(bool bActive);
	void AddLooseGameplayTag(FGameplayTag TagName);
	void RemoveLooseGameplayTag(FGameplayTag TagName);
	TObjectPtr<UTPS_CharacterMovementComponent> GetTPSCharacterMovementComp() const;
};
