// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPS_CharacterStateComponent.generated.h"


UENUM(BlueprintType)
enum class ECharacterWeaponState : uint8
{
	Sheathing,
	Sheathed,
	Drawing,
	Drawn,
	DrawAttack
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THIRDPERSON_FUNCTION_API UTPS_CharacterStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	ECharacterWeaponState WeaponState;
	// Sets default values for this component's properties
	UTPS_CharacterStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponStateChanged, ECharacterWeaponState);
	FOnWeaponStateChanged OnWeaponStateChanged;

public:
	void SetWeaponState(ECharacterWeaponState NewState, bool bforceChange = false);
	ECharacterWeaponState GetWeaponState() const { return WeaponState; }
};
