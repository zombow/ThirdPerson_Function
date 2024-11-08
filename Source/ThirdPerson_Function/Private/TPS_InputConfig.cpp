// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_InputConfig.h"

FGameplayTag UTPS_InputConfig::GetTagForAction(const TObjectPtr<UInputAction>& InputAction)
{
	const FGameplayTag* FoundTag = InputActionTagMap.Find(InputAction);
	return FoundTag ? *FoundTag : FGameplayTag();
}
