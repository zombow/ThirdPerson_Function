// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TPS_Player/TPS_InputConfig.h"

void ATPS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputConfig->GetImc(), 0);
	}
}

void ATPS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	BindInputDataSet(InputConfig->GetInputActionTagMap());
}

void ATPS_PlayerController::HandleInputs(const FInputActionInstance& Instance, FGameplayTag Tag)
{
	OnInputs.Broadcast(Instance, Tag);
}

void ATPS_PlayerController::BindInputDataSet(TMap<FGameplayTag, TObjectPtr<UInputAction>>* InputActionTagMap)
{
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		for (const TPair<FGameplayTag, TObjectPtr<UInputAction>>& Pair : *InputActionTagMap)
		{
			EnhancedInputComponent->BindAction(Pair.Value, ETriggerEvent::Triggered, this,
			                                   &ATPS_PlayerController::HandleInputs, Pair.Key);
		}

	}
}

