// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPS_GameplayAbility_SheathWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_GameplayAbility_SheathWeapon : public UGameplayAbility
{
	GENERATED_BODY()
	UTPS_GameplayAbility_SheathWeapon();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void PlayMontage();
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSheathIn);
	FSheathIn SheathIn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "AnimMontage")
	TObjectPtr<UAnimMontage> SheathMontage;
};
