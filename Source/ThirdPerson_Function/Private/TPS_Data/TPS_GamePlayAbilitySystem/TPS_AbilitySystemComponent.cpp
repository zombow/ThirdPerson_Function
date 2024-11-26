// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AttributeSet.h"

void UTPS_AbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	// InitStats(Cast<UTPS_AttributeSet>(GetAttributeSet(UTPS_AttributeSet::StaticClass()))->StaticClass(), DefaultStartingData.GetData()->DefaultStartingTable);
}
