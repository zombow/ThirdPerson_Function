// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Animation/TPS_ComboAttackAnimNotifyState.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Character.h"
#include "TPS_Animation/TPS_AnimInstance.h"


void UTPS_ComboAttackAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ACharacter* Target = Cast<ACharacter>(MeshComp->GetOwner());
	
	if (auto AnimInstance = Cast<UTPS_AnimInstance>(MeshComp->GetAnimInstance()))
	{
		FString NextSection = (FString::Printf(TEXT("%s"), *AnimInstance->Montage_GetCurrentSection().ToString()));
		NextSection[NextSection.Len() - 1] = NextSection[NextSection.Len() - 1] + 1;
		if (AnimInstance->GetCurrentActiveMontage()->GetNumSections() < (NextSection[NextSection.Len() - 1] - '0'))
		{
			NextSection[NextSection.Len() - 1] = '1';
		}
		NextSectionTag = FGameplayTag::RequestGameplayTag(*NextSection);
		
		FGameplayEventData EventData; // ComboAttack Event생성
		EventData.EventTag = NextSectionTag;
		EventData.Instigator = Target;
		auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}

void UTPS_ComboAttackAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (ACharacter* Target = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		FGameplayEventData EventData; // ComboAttackEnd Event생성
		EventData.EventTag = FGameplayTag::RequestGameplayTag("State.Character.EndAttack");
		EventData.Instigator = Target;
		auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		ASC->HandleGameplayEvent(EventData.EventTag, &EventData);
	}
}
