// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "TPS_Animation/TPS_AnimInstance.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/TPS_AbilitySystemComponent.h"
#include "TPS_Player/TPS_CharacterStateComponent.h"
#include "TPS_GameplayAbility_DrawWeapon.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_GameplayAbility_DrawWeapon : public UGameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<ACharacter> Target;
	UPROPERTY()
	TObjectPtr<UTPS_AnimInstance> PlayerAnimInstance;
	UPROPERTY()
	TObjectPtr<UTPS_AbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UTPS_CharacterStateComponent> CharacterStateComponent;
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
