// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TPS_SetRootModeAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class THIRDPERSON_FUNCTION_API UTPS_SetRootModeAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

protected:

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
