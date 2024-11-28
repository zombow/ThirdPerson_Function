// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TPS_AttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FGameplayAttributeData MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FGameplayAttributeData CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	FGameplayAttributeData MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	FGameplayAttributeData CurrentStamina;

public:
	ATTRIBUTE_ACCESSORS(UTPS_AttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UTPS_AttributeSet, CurrentHealth);
	ATTRIBUTE_ACCESSORS(UTPS_AttributeSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UTPS_AttributeSet, CurrentStamina);
};
