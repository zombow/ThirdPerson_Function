// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Animation/TPS_AllowRotationAnimNotifyState.h"

#include "TPS_Player/TPS_PlayerCharacter.h"

void UTPS_AllowRotationAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if(auto Player = Cast<ATPS_PlayerCharacter>(MeshComp->GetOwner()))
		Player->GetTPSCharacterMovementComp()->bAllowPhysicsRotationDuringAnimRootMotion = false;
}

void UTPS_AllowRotationAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if(auto Player = Cast<ATPS_PlayerCharacter>(MeshComp->GetOwner()))
		Player->GetTPSCharacterMovementComp()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}
