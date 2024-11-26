// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerState.h"

ATPS_PlayerState::ATPS_PlayerState()
{
	TPS_AbilitySystemComp = CreateDefaultSubobject<UTPS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	//TPS_AttributeSet = CreateDefaultSubobject<UTPS_AttributeSet>(TEXT("AttributeSet"));
}

TObjectPtr<UTPS_AbilitySystemComponent> ATPS_PlayerState::GetAbilitySystemComponent() const
{
	return TPS_AbilitySystemComp;
}
