// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Animation/TPS_AnimInstance.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimInstanceProxy.h"
#include "TPS_Player/TPS_PlayerCharacter.h"

void UTPS_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	TPSPlayerController = Cast<ATPS_PlayerController>(TryGetPawnOwner()->GetController());
	if (TPSPlayerController)
	{
		TPSPlayerController->OnMoveOngoing.AddDynamic(this, &UTPS_AnimInstance::MoveOn);
		TPSPlayerController->OnMoveEnd.AddDynamic(this, &UTPS_AnimInstance::MoveEnded);
	}
}

void UTPS_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UTPS_AnimInstance::MoveOn(FInputActionInstance Value)
{
	this->MoveOngoing(Value);
}

void UTPS_AnimInstance::MoveOngoing_Implementation(FInputActionInstance Value)
{
}

void UTPS_AnimInstance::MoveEnd(FInputActionInstance Value)
{
	this->MoveEnded(Value);
}

void UTPS_AnimInstance::MoveEnded_Implementation(FInputActionInstance Value)
{
}
