// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Jump.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GEffect_JumpCost.h"

UTPS_GameplayAbility_Jump::UTPS_GameplayAbility_Jump()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Jump")));
	CostGameplayEffectClass = UTPS_GEffect_JumpCost::StaticClass();
}

void UTPS_GameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (auto player = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		if (player->GetCharacterMovement()->DoJump(true) && !CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			// TODO: 착지 or 다음동작이 가능할때
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
	else
	{
		// Character가아닌경우
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}
