// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsSettings.h"
#include "NativeGameplayTags.h"
#include "TPS_Data/TPS_GamePlayAbilitySystem/Abilities/TPS_GameplayAbility_Jump.h"
#include "TPS_Player/TPS_PlayerController.h"
#include "TPS_Player/TPS_PlayerState.h"

// Sets default values
ATPS_PlayerCharacter::ATPS_PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTPS_CharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationYaw = false;
	// 플레이어 스켈레톤 설정
	PlayerSkeletalMeshComp = FindComponentByClass<USkeletalMeshComponent>();
	if (LoadObject<USkeletalMesh>(
		nullptr,TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'")))
	{
		PlayerSkeletalMeshComp->SetSkeletalMesh(LoadObject<USkeletalMesh>(
			nullptr,TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'")));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Find SkeletalMesh..."));
	}
	// TODO:load좌표를 다른곳에서 관리

	// 카메라붐 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	CameraBoom->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f), false);
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = false;

	// 카메라 설정
	PlayerCameraComp = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCameraComp->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PlayerCameraComp->bUsePawnControlRotation = false;

	// 무브먼트 설정
	TPSCharacterMoveComp = FindComponentByClass<UTPS_CharacterMovementComponent>();
	TPSCharacterMoveComp->bUseControllerDesiredRotation = true; // 컨트롤 회전값으로 캐릭터회전 활성화
	TPSCharacterMoveComp->bOrientRotationToMovement = true; // 이동방향으로 캐릭터회전 활성화
	TPSCharacterMoveComp->NavAgentProps.bCanCrouch = true; // 앉기기능 활성화
	TPSCharacterMoveComp->RotationRate = FRotator(0.0f, 460.0f, 0.0f);
	TPSCharacterMoveComp->MaxWalkSpeed = 600.0f;
	TPSCharacterMoveComp->MaxWalkSpeedCrouched = 300.0f;

	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ATPS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (TObjectPtr<ATPS_PlayerController> TPSController = Cast<ATPS_PlayerController>(Controller))
	{
		TPSController->OnMoveInput.AddDynamic(this, &ATPS_PlayerCharacter::Move);
		TPSController->OnJumpInput.AddDynamic(this, &ATPS_PlayerCharacter::DoJump);
		TPSController->OnCrouching.AddDynamic(this, &ATPS_PlayerCharacter::Crouching);
		TPSController->UnCrouching.AddDynamic(this, &ATPS_PlayerCharacter::UnCrouching);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("-----Can't Find PlayerController In [%s]-------"), *this->GetName());
	}
}

void ATPS_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// ASC할당
	if (!TPSAbilitySystemComp)
	{
		if (TPSAbilitySystemComp = GetPlayerState<ATPS_PlayerState>()->GetAbilitySystemComponent())
		{
			TPSAbilitySystemComp->InitAbilityActorInfo(GetPlayerState<ATPS_PlayerState>(), this);

			// Ability 바인딩
			const FGameplayAbilitySpec AbilitySpec(UTPS_GameplayAbility_Jump::StaticClass(), 1);
			TPSAbilitySystemComp->GiveAbility(AbilitySpec);
		}
	}
}

UAbilitySystemComponent* ATPS_PlayerCharacter::GetAbilitySystemComponent() const
{
	return TPSAbilitySystemComp;
}

TObjectPtr<UTPS_CharacterMovementComponent> ATPS_PlayerCharacter::GetTPSCharacterMovementComp() const
{
	return TPSCharacterMoveComp;
}

void ATPS_PlayerCharacter::Move(FVector2D Value)
{
	FVector2D InputDirection = Value;

	FRotator CameraRotation = CameraBoom->GetComponentRotation();
	FVector CameraForward = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
	CameraForward.Z = 0;

	FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
	CameraRight.Z = 0;


	FVector MoveDirection = FVector((CameraForward * InputDirection.X) + (CameraRight * InputDirection.Y));
	MoveDirection.Normalize();
	AddMovementInput(MoveDirection);
}

void ATPS_PlayerCharacter::DoJump()
{
	TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Jump"))));
}

void ATPS_PlayerCharacter::Crouching()
{
	if (TPSCharacterMoveComp->IsMovingOnGround())
		ATPS_PlayerCharacter::Crouch();
}

void ATPS_PlayerCharacter::UnCrouching()
{
	ATPS_PlayerCharacter::UnCrouch();
}
