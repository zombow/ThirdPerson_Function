// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Animation/TPS_AnimInstance.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "TPS_Player/TPS_PlayerCharacter.h"

void UTPS_AnimInstance::AnimNotify_NextAttack()
{
	auto Player = Cast<ATPS_PlayerCharacter>(GetOwningActor());
	FGameplayEventData EventData; // Attack종료 Event생성
	FString Tag = (FString::Printf(TEXT("%s"), *Montage_GetCurrentSection().ToString()));
	EventData.EventTag = FGameplayTag::RequestGameplayTag(*Tag);
	EventData.Instigator = Player;
	Player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
}
