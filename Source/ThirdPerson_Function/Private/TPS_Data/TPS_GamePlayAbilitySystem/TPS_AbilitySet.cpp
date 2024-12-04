// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySet.h"

TMap<FGameplayTag, TSubclassOf<UGameplayAbility>> UTPS_AbilitySet::GetTPSAbilities()
{
	return TPSAbilities;
}
