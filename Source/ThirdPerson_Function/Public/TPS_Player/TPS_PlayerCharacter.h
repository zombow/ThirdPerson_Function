// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "TPS_InteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPS_Player/TPS_CharacterMovementComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySet.h"
#include "TPS_Props/TPS_InteractableActor.h"
#include "TPS_PlayerCharacter.generated.h"

UCLASS()
class ATPS_PlayerCharacter : public ACharacter, public IAbilitySystemInterface
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GAS")
	TObjectPtr<UTPS_AbilitySet> TPSAbilities;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TObjectPtr<UTPS_InteractionComponent> TPSInteractionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TObjectPtr<UBoxComponent> TargetInteractionBox;

	// direction
	FVector TPSLastInput;
	FVector DesiredDirection;


	void AbilityBind(TSubclassOf<UGameplayAbility>& AbilityClass, FGameplayTag AbilityTag, int Level);
	TMap<FGameplayTag, FGameplayAbilitySpec> AbilitySpecs;

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
	UFUNCTION()
	void SheathWeapon();
	UFUNCTION()
	void Interaction();
	
	FRotator CurrentActorRotation;
	bool bTurning;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TSet<TObjectPtr<ATPS_InteractableActor>> InteractableActorArray;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Interaction")
	TObjectPtr<ATPS_InteractableActor> FocusdInteractableActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true), Category = "Jump")
	bool bPressedJumpKey;

	
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UFUNCTION(BlueprintCallable)
	FVector GetTPSLastInput();

	void StaminaRegen(bool bActive);
	FGameplayAbilitySpec* GetAbilitySpec(FGameplayTag AbilityTag);
	void AddLooseGameplayTag(FGameplayTag TagName);
	void RemoveLooseGameplayTag(FGameplayTag TagName);
	TObjectPtr<UTPS_CharacterMovementComponent> GetTPSCharacterMovementComp() const;
};
