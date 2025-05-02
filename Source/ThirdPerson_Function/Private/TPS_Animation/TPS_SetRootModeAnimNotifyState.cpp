// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Animation/TPS_SetRootModeAnimNotifyState.h"


void UTPS_SetRootModeAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	UE_LOG(LogTemp, Warning, TEXT("Begin: %s"), *Animation->GetName());
}

void UTPS_SetRootModeAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	UE_LOG(LogTemp, Warning, TEXT("END: %s"), *Animation->GetName());
}
