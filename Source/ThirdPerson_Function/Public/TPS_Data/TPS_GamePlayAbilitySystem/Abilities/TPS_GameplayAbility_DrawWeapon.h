// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TPS_Animation/TPS_AnimInstance.h"
#include "TPS_Player/TPS_PlayerCharacter.h"
#include "TPS_GameplayAbility_DrawWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_GameplayAbility_DrawWeapon : public UGameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<ATPS_PlayerCharacter> Player;
	UPROPERTY()
	TObjectPtr<UTPS_AnimInstance> PlayerAnimInstance;
	
	UTPS_GameplayAbility_DrawWeapon();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void PlayMontage();
	UFUNCTION()
	void OnMontageBlendOut();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();
	UFUNCTION()
	void KeepPlayMontage();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "AnimMontage")
	TObjectPtr<UAnimMontage> DrawMontage;
};
