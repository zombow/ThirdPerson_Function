// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPS_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Float")
	float TESTFloat;
};
