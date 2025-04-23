// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Props/TPS_InteractableActor.h"

#include "TPS_Player/TPS_PlayerCharacter.h"

// Sets default values
ATPS_InteractableActor::ATPS_InteractableActor()
{

	PrimaryActorTick.bCanEverTick = false;

}


void ATPS_InteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPS_InteractableActor::InteractionStart(ATPS_PlayerCharacter& PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter : %p"), *PlayerCharacter.GetName() )
}


