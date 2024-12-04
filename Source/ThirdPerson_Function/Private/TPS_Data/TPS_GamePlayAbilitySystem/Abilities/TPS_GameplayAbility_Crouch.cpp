// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Crouch.h"

#include "TPS_Player/TPS_PlayerCharacter.h"

UTPS_GameplayAbility_Crouch::UTPS_GameplayAbility_Crouch()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Crouch")));
}

void UTPS_GameplayAbility_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		if (player->GetCharacterMovement()->IsMovingOnGround() && !player->GetCharacterMovement()->IsCrouching())
		{
			player->Crouch();
			UE_LOG(LogTemp,Warning,TEXT("Crouch"));
			// TODO:  앉기가끝나고 다음동작이 가능할때
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
		else if(player->GetCharacterMovement()->IsCrouching())
		{
			UE_LOG(LogTemp,Warning,TEXT("UnCrouch"));
			player->UnCrouch();
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
	else
	{
		// Character가아닌경우
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}
