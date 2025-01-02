// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_GEExecutionCalc_StaminaRegen.h"

#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AttributeSet.h"

void UTPS_GEExecutionCalc_StaminaRegen::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	TObjectPtr<UTPS_AbilitySystemComponent> TargetASC = Cast<UTPS_AbilitySystemComponent>(ExecutionParams.GetTargetAbilitySystemComponent());
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	float StaminaIncreaseRate = TargetASC->GetOwner()->GetWorld()->GetDeltaSeconds();
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(UTPS_AttributeSet::GetCurrentStaminaAttribute(), EGameplayModOp::Additive, StaminaIncreaseRate));

}
