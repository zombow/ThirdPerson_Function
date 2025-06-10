// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_DrawWeapon.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UTPS_GameplayAbility_DrawWeapon::UTPS_GameplayAbility_DrawWeapon()
{
	AbilityTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")));
}


void UTPS_GameplayAbility_DrawWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                      const FGameplayAbilityActivationInfo ActivationInfo,
                                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Target = Cast<ACharacter>(ActorInfo->AvatarActor);
	if (Target)
	{
		PlayerAnimInstance = Cast<UTPS_AnimInstance>(Target->GetMesh()->GetAnimInstance());
		if (Target->GetClass()->ImplementsInterface(UAbilitySystemInterface::StaticClass()))
		{
			auto GAS = Cast<IAbilitySystemInterface>(Target);
			if (GAS->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn"))))
			{
				CachedMontage = SheathMontage;
			}
			else
			{
				CachedMontage = DrawMontage;
			}
			PlayMontage();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPS_GameplayAbility_DrawWeapon::PlayMontage()
{
	if (Target)
	{
		PlayerAnimInstance->bisPlayingMontage = true;
	}

	TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		CachedMontage,
		1.0f,
		CachedMontage->CompositeSections[0].SectionName,
		true
	);
	Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::KeepPlayMontage);
	Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted);
	Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled);
	Task->ReadyForActivation();
	UE_LOG(LogTemp, Warning, TEXT("Play Montage : %s"), *CachedMontage->GetName());
}

void UTPS_GameplayAbility_DrawWeapon::KeepPlayMontage()
{
	auto GAS = Cast<IAbilitySystemInterface>(Target);
	if (CachedMontage == DrawMontage)
	{
		GAS->GetAbilitySystemComponent()->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
	}
	else
	{
		GAS->GetAbilitySystemComponent()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn")));
	}

	if (GAS->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.DrawAttack")))
		OnMontageBlendOut();
	else
	{
		TObjectPtr<UAbilityTask_PlayMontageAndWait> Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			CachedMontage,
			1.0f,
			CachedMontage->CompositeSections[1].SectionName,
			true
		);

		Task->OnBlendOut.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageBlendOut);
		Task->OnInterrupted.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted);
		Task->OnCancelled.AddDynamic(this, &UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled);
		Task->ReadyForActivation();
	}
}

void UTPS_GameplayAbility_DrawWeapon::OnMontageBlendOut()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_DrawWeapon::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_DrawWeapon::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_DrawWeapon::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (Target)
	{
		PlayerAnimInstance->bisPlayingMontage = false;
		/** 잠시 보류
		FGameplayEventData EventData; // Draw종료 Event생성
		EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("State.Character.Drawn"));
		EventData.Instigator = Target;
		Target->GetAbilitySystemComponent()->HandleGameplayEvent(EventData.EventTag, &EventData);
		**/
	}
}
