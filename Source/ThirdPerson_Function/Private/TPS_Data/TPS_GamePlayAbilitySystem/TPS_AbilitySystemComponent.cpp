// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AttributeSet.h"


FGameplayAbilitySpec* UTPS_AbilitySystemComponent::GetAbilitySpecFromTag(FGameplayTag AbilityTag)
{
	if (auto AbilitySpec = TPSAbilitySpecs.Find(AbilityTag))
	{
		return AbilitySpec;
	}
	return nullptr;
}

void UTPS_AbilitySystemComponent::InitializeFromAbilitySet(UTPS_AbilitySet* AbilitySet)
{
	if (!AbilitySet) return;

	for (auto& Pair : AbilitySet->GetTPSAbilities())
	{
		FGameplayAbilitySpec Spec(Pair.Value, 1);
		GiveAbility(Spec);
		TPSAbilitySpecs.Add(Pair.Key, Spec);
	}
}
