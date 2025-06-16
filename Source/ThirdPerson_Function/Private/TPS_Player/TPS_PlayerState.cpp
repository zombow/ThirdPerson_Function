// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerState.h"


ATPS_PlayerState::ATPS_PlayerState()
{
	NetUpdateFrequency = 100;
	TPSAbilitySystemComp = CreateDefaultSubobject<UTPS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	TPSAbilitySystemComp->SetIsReplicated(true);
	TPSAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void ATPS_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystem();
	AbilityBind();
	if (auto StateComp = GetPawn()->FindComponentByClass<UTPS_CharacterStateComponent>())
	{
		TPSAbilitySystemComp->InitCharacterStateComponent(StateComp);
	}
}

TObjectPtr<UTPS_AbilitySystemComponent> ATPS_PlayerState::GetAbilitySystemComponent() const
{
	return TPSAbilitySystemComp;
}

void ATPS_PlayerState::InitAbilitySystem()
{
	if (!TPSAbilitySystemComp) return;

	TPSAbilitySystemComp->InitAbilityActorInfo(this, GetPawn());
}

void ATPS_PlayerState::AbilityBind()
{
	if (!TPSAbilitySystemComp) return;

	TPSAbilitySystemComp->InitializeFromAbilitySet(TPSAbilities);
}
