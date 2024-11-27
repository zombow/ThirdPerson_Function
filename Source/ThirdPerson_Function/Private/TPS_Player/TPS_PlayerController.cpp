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
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Look"))), ETriggerEvent::Triggered,
		                                   this, &ATPS_PlayerController::HandleMouseMoveInput);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Move"))), ETriggerEvent::Triggered,
		                                   this, &ATPS_PlayerController::HandleMoveInput);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Jump"))), ETriggerEvent::Triggered,
										   this, &ATPS_PlayerController::HandleJumpInput);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Crouch"))), ETriggerEvent::Started,
										   this, &ATPS_PlayerController::HandleOnCrouching);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Crouch"))), ETriggerEvent::Completed,
										   this, &ATPS_PlayerController::HandleUnCrouching);
	}
}

void ATPS_PlayerController::HandleMouseMoveInput(const FInputActionValue& Value)
{
	OnMouseMoveInput.Broadcast(Value.Get<FVector2D>());
}

void ATPS_PlayerController::HandleMoveInput(const FInputActionValue& Value)
{
	OnMoveInput.Broadcast(Value.Get<FVector2D>());
}

void ATPS_PlayerController::HandleJumpInput(const FInputActionValue& Value)
{
	OnJumpInput.Broadcast();
}

void ATPS_PlayerController::HandleOnCrouching(const FInputActionValue& Value)
{
	OnCrouching.Broadcast();
}

void ATPS_PlayerController::HandleUnCrouching(const FInputActionValue& Value)
{
	UnCrouching.Broadcast();
}
