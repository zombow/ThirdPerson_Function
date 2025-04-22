// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_InteractionComponent.h"

#include "TPS_Player/TPS_PlayerCharacter.h"

// Sets default values for this component's properties
UTPS_InteractionComponent::UTPS_InteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTPS_InteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionBox)
	{
		InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &UTPS_InteractionComponent::OnOverlapBegin);
		InteractionBox->OnComponentEndOverlap.AddDynamic(this, &UTPS_InteractionComponent::OnOverlapEnd);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("InteractionBox is NOT FOUND"))
	}
}


void UTPS_InteractionComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin Overlap"));
}

void UTPS_InteractionComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                             int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("End Overlap"));
}


// Called every frame
void UTPS_InteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
