// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"
#include "Abilities/GameplayAbility.h"
#include "TPS_Animation/TPS_AnimInstance.h"
#include "TPS_Player/TPS_PlayerCharacter.h"
#include "TPS_GameplayAbility_Attack.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_GameplayAbility_Attack : public UGameplayAbility
{
	GENERATED_BODY()
	UTPS_GameplayAbility_Attack();

	UPROPERTY()
	TObjectPtr<ATPS_PlayerCharacter> Player;
	UPROPERTY()
	TObjectPtr<UTPS_AnimInstance> PlayerAnimInstance;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void PlayMontage(FGameplayTag SectionName = FGameplayTag());
	UFUNCTION()
	void OnMontageBlendOut();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();

	void Attack();

	void BeginNextAttackState(const FGameplayTag EventTag, const FGameplayEventData* Payload);
	void EndNextAttackState(const FGameplayTag EventTag, const FGameplayEventData* Payload);


	void DrawEndHandle(const FGameplayTag EventTag, const FGameplayEventData* Payload = nullptr);

	FDelegateHandle DrawEventHandle;
	FDelegateHandle AttackEventHandle;
	FDelegateHandle EndAttackEventHandle;
	FGameplayTag NextSectionTag;
	bool bNextAttack = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "AnimMontage")
	TObjectPtr<UAnimMontage> AttackMontage;
};
