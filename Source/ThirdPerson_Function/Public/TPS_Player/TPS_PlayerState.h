// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AttributeSet.h"
#include "TPS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API ATPS_PlayerState : public APlayerState
{
	GENERATED_BODY()

	ATPS_PlayerState();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GAS")
	TObjectPtr<UTPS_AbilitySystemComponent> TPS_AbilitySystemComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "GAS")
	TObjectPtr<UTPS_AttributeSet> TPS_AttributeSet;
	
	TObjectPtr<UTPS_AbilitySystemComponent> GetAbilitySystemComponent() const;
};
