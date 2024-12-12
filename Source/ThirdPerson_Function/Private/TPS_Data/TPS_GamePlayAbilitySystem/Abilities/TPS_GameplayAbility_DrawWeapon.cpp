// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_DrawWeapon.h"

#include "TPS_Player/TPS_PlayerCharacter.h"

UTPS_GameplayAbility_DrawWeapon::UTPS_GameplayAbility_DrawWeapon()
{
	AbilityTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")));
}

void UTPS_GameplayAbility_DrawWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		player->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPS_GameplayAbility_DrawWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		player->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
	}
}
