// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerState.h"

ATPS_PlayerState::ATPS_PlayerState()
{
	NetUpdateFrequency = 100;
	TPSAbilitySystemComp = CreateDefaultSubobject<UTPS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	TPSAbilitySystemComp->SetIsReplicated(true);
	TPSAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
}

TObjectPtr<UTPS_AbilitySystemComponent> ATPS_PlayerState::GetAbilitySystemComponent() const
{
	return TPSAbilitySystemComp;
}
