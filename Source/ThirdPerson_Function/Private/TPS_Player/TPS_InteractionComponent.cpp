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
	if (TObjectPtr<ATPS_InteractableActor> InteractableActor = Cast<ATPS_InteractableActor>(OtherActor))
	{
		Player->InteractableActorArray.Add(InteractableActor);
		Player->FocusdInteractableActor = FindNearActor(Player->GetActorLocation(), Player->InteractableActorArray);
	}
}

void UTPS_InteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex)
{
	if (TObjectPtr<ATPS_InteractableActor> InteractableActor = Cast<ATPS_InteractableActor>(OtherActor))
	{
		Player->InteractableActorArray.Remove(InteractableActor);
		Player->FocusdInteractableActor = FindNearActor(Player->GetActorLocation(), Player->InteractableActorArray);
	}
}

ATPS_InteractableActor* UTPS_InteractionComponent::FindNearActor(const FVector& Origin, const TSet<TObjectPtr<ATPS_InteractableActor>>& ActorSet)
{
	ATPS_InteractableActor* NearActor = nullptr;
	float DistanceFromNearestActor = TNumericLimits<float>::Max();
	for (TObjectPtr<ATPS_InteractableActor> ActorToCheck : ActorSet)
	{
		if (ActorToCheck)
		{
			const float DistanceFromActorToCheck = (Origin - ActorToCheck->GetActorLocation()).SizeSquared();
			if (DistanceFromActorToCheck < DistanceFromNearestActor)
			{
				NearActor = ActorToCheck;
				DistanceFromNearestActor = DistanceFromActorToCheck;
			}
		}
	}
	return NearActor;
}

