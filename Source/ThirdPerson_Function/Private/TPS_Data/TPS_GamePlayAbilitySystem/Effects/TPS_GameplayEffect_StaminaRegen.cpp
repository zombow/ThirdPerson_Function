// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_StaminaRegen.h"

#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AttributeSet.h"

UTPS_GameplayEffect_StaminaRegen::UTPS_GameplayEffect_StaminaRegen()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 1;
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UTPS_AttributeSet::GetCurrentStaminaAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(10);

	Modifiers.Add(ModifierInfo);
}

void UTPS_GameplayEffect_StaminaRegen::SetModifier(float DeltaSeconds, float ScaleFloat)
{

}
