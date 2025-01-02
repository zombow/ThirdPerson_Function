// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_StaminaRegen.h"

#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AttributeSet.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_GEExecutionCalc_StaminaRegen.h"

UTPS_GameplayEffect_StaminaRegen::UTPS_GameplayEffect_StaminaRegen()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;
	Period = 0.1f;
	// 미사용 Execution
	// FGameplayEffectExecutionDefinition ExecutionDef;
	// ExecutionDef.CalculationClass = UTPS_GEExecutionCalc_StaminaRegen::StaticClass();
	// Executions.Add(ExecutionDef);

	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UTPS_AttributeSet::GetCurrentStaminaAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(0.5f);
	
	Modifiers.Add(ModifierInfo);
}
