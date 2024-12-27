// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPS_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bisPlayingMontage;

	UFUNCTION()
	void AnimNotify_NextAttack();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNextAttack);
	FNextAttack NextAttack;
	
};
