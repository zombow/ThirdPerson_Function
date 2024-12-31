// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Animation/TPS_ComboAttackAnimNotifyState.h"

#include "TPS_Animation/TPS_AnimInstance.h"
#include "TPS_Player/TPS_PlayerCharacter.h"


void UTPS_ComboAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	auto Player = Cast<ATPS_PlayerCharacter>(MeshComp->GetOwner());
	auto AnimInstance = Cast<UTPS_AnimInstance>(MeshComp->GetAnimInstance());
	if (AnimInstance)
	{
		FString NextSection = (FString::Printf(TEXT("%s"), *AnimInstance->Montage_GetCurrentSection().ToString()));
		NextSection[NextSection.Len() - 1] = NextSection[NextSection.Len() - 1] + 1;
		if (AnimInstance->GetCurrentActiveMontage()->GetNumSections() < (NextSection[NextSection.Len() - 1] - '0'))
		{
			NextSection[NextSection.Len() - 1] = '1';
		}
		NextSectionTag = FGameplayTag::RequestGameplayTag(*NextSection);
		bCanComboAttack = true;
		
		FGameplayEventData EventData; // ComboAttack Event생성
		EventData.EventTag = NextSectionTag;
		EventData.Instigator = Player;
		Player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void UTPS_ComboAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	auto Player = Cast<ATPS_PlayerCharacter>(MeshComp->GetOwner());
	if (Player)
	{
		FGameplayEventData EventData; // ComboAttackEnd Event생성
		EventData.EventTag = FGameplayTag::RequestGameplayTag("State.Character.EndAttack");
		EventData.Instigator = Player;
		Player->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
		bCanComboAttack = false;
	}
}
