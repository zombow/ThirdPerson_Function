// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Jump.h"

#include "GameFramework/MovementComponent.h"
#include "TPS_Player/TPS_PlayerCharacter.h"

UTPS_GameplayAbility_Jump::UTPS_GameplayAbility_Jump()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
}

void UTPS_GameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo,
			true, true);
	}
}

void UTPS_GameplayAbility_Jump::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);

	if(auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		player->GetTPSCharacterMovementComp()->DoJump(true);
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}
