// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPS_InteractableActor.generated.h"

class ATPS_PlayerCharacter;

UCLASS()
class THIRDPERSON_FUNCTION_API ATPS_InteractableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATPS_InteractableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void InteractionStart(ATPS_PlayerCharacter& PlayerCharacter);
};
