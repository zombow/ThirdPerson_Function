// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_InputConfig.h"

TObjectPtr<UInputAction> UTPS_InputConfig::GetAction(FGameplayTag Tag)
{
	auto temp = InputActionTagMap.FindChecked(Tag);
	return temp;
}

TObjectPtr<UInputMappingContext> UTPS_InputConfig::GetImc()
{
	return InputMappingContext;
}
