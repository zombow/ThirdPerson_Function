// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_JumpCost.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AttributeSet.h"

UTPS_GameplayEffect_JumpCost::UTPS_GameplayEffect_JumpCost()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UTPS_AttributeSet::GetCurrentStaminaAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(-10.0f);
	
	Modifiers.Add(ModifierInfo);
}
