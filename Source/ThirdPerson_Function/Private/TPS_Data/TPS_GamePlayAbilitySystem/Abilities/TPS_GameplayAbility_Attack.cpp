#include "TPS_Player/TPS_PlayerCharacter.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Attack.h"

#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_DrawWeapon.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Effects/TPS_GameplayEffect_AttackCost.h"

UTPS_GameplayAbility_Attack::UTPS_GameplayAbility_Attack()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Attack")));
	CostGameplayEffectClass = UTPS_GameplayEffect_AttackCost::StaticClass();
}

void UTPS_GameplayAbility_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                                  const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		player->GetAbilitySystemComponent()->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Ability.DrawWeapon")));
		UGameplayAbility* Target = player->GetAbilitySystemComponent()->FindAbilitySpecFromClass(UTPS_GameplayAbility_DrawWeapon::StaticClass()
		)->Ability;
		if (UTPS_GameplayAbility_DrawWeapon* DrawAbility = Cast<UTPS_GameplayAbility_DrawWeapon>(Target))
		{
			DrawAbility->DrawIn.AddDynamic(this, &UTPS_GameplayAbility_Attack::Attack);
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}
}

void UTPS_GameplayAbility_Attack::Attack()
{
	if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
		Cast<ATPS_PlayerCharacter>(CurrentActorInfo->AvatarActor)->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UTPS_GameplayAbility_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (auto player = Cast<ATPS_PlayerCharacter>(ActorInfo->AvatarActor))
	{
		player->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Character.Attack"));
		UE_LOG(LogTemp, Warning, TEXT("End Attack"));
	}
}


void UTPS_GameplayAbility_Attack::PlayMontage()
{
}

void UTPS_GameplayAbility_Attack::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTPS_GameplayAbility_Attack::OnMontageInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UTPS_GameplayAbility_Attack::OnMontageCancelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
