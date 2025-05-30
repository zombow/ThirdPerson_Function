// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "UObject/Interface.h"
#include "ControllerInput.generated.h"



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UControllerInput : public UInterface
{
	GENERATED_BODY()
};

UENUM()
enum class EInputActionType : uint8
{
	Look,
	Move,
	Jump,
	Crouch,
	Roll,
	Attack,
	DrawWeapon,
	SheathWeapon,
	Interaction,
	Max UMETA(Hidden)
};

/**
 * 
 */
class THIRDPERSON_FUNCTION_API IControllerInput
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	

public:
	TDelegate<void(const FInputActionInstance&)> InputDelegates[static_cast<uint8>(EInputActionType::Max)]; // binding 해줄 델리게이트
	void HandleInput(EInputActionType type, const FInputActionInstance& Value); // enum을 이용해 분기
};
