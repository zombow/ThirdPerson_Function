// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_InteractionComponent.h"

#include "Kismet/BlueprintTypeConversions.h"
#include "Kismet/GameplayStatics.h"
#include "TPS_Player/TPS_PlayerCharacter.h"
#include "TPS_Props/TPS_InteractableActor.h"


// Sets default values for this component's properties
UTPS_InteractionComponent::UTPS_InteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UTPS_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionBox)
	{
		Player = Cast<ATPS_PlayerCharacter>(GetOwner());
		InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &UTPS_InteractionComponent::OnOverlapBegin);
		InteractionBox->OnComponentEndOverlap.AddDynamic(this, &UTPS_InteractionComponent::OnOverlapEnd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InteractionBox is NOT FOUND"))
	}
}

void UTPS_InteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Overlap"));
	if (TObjectPtr<ATPS_InteractableActor> InteractableActor = Cast<ATPS_InteractableActor>(OtherActor))
	{
		Player->InteractableActorArray.Add(InteractableActor);
		TObjectPtr<ATPS_InteractableActor> FocusedObject = nullptr;
		float DistanceFromNearestActor = TNumericLimits<float>::Max();
		for (TObjectPtr<ATPS_InteractableActor> ActorToCheck : Player->InteractableActorArray)
		{
			if (ActorToCheck)
			{
				const float DistanceFromActorToCheck = (Player->GetActorLocation() - ActorToCheck->GetActorLocation()).SizeSquared();
				if (DistanceFromActorToCheck < DistanceFromNearestActor)
				{
					FocusedObject = ActorToCheck;
					DistanceFromNearestActor = DistanceFromActorToCheck;
				}
			}
		}
		Player->FocusdInteractableActor = FocusedObject;
	}
}

void UTPS_InteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("End Overlap"));
	if (TObjectPtr<ATPS_InteractableActor> InteractableActor = Cast<ATPS_InteractableActor>(OtherActor))
	{
		Player->InteractableActorArray.Remove(InteractableActor);
	}
}
