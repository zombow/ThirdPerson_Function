// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Jump.h"

#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_JumpCost.h"
#include "GameFramework/CharacterMovementComponent.h"

UTPS_GameplayAbility_Jump::UTPS_GameplayAbility_Jump()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
	CostGameplayEffectClass = UTPS_GameplayEffect_JumpCost::StaticClass();
}

void UTPS_GameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor);
	if (Player)
	{
		Player->GetCharacterMovement()->DoJump(true) && CommitAbility(Handle, ActorInfo, ActivationInfo);
		Player->bPressedJumpKey = true;
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UTPS_GameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	Player->bPressedJumpKey = false;

}
