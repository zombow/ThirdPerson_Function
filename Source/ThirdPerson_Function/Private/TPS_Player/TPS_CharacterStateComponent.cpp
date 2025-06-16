// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_CharacterStateComponent.h"

// Sets default values for this component's properties
UTPS_CharacterStateComponent::UTPS_CharacterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UTPS_CharacterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UTPS_CharacterStateComponent::SetWeaponState(ECharacterWeaponState NewState, bool bforceChange)
{
	if (WeaponState != NewState)
	{
		if (!bforceChange)
		{
			if (WeaponState == ECharacterWeaponState::DrawAttack && NewState == ECharacterWeaponState::Drawing)
			{
				return; // DrawAttack도중에는 자동으로 Draw로 변경할수없음
			}
		}
		WeaponState = NewState;
		OnWeaponStateChanged.Broadcast(WeaponState);
	}
}
