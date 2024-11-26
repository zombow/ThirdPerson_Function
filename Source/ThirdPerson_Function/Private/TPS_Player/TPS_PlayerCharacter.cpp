// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagsSettings.h"
#include "NativeGameplayTags.h"
#include "TPS_Player/TPS_PlayerState.h"

// Sets default values
ATPS_PlayerCharacter::ATPS_PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTPS_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
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
	
	// ASC할당
	TPSAbilitySystemComp = GetPlayerState<ATPS_PlayerState>()->GetAbilitySystemComponent();

	// Enhanced Input 시스템 설정 (PlayerController에서 Enhanced Input Subsystem 사용)
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputConfig->GetImc(), 0);
		}
	}

}

// Called every frame
void ATPS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// Enhanced Input 컴포넌트로 캐스팅
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<
		UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Look"))), ETriggerEvent::Triggered, this, &ATPS_PlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Move"))), ETriggerEvent::Triggered, this, &ATPS_PlayerCharacter::Move);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Jump"))), ETriggerEvent::Triggered, this, &ATPS_PlayerCharacter::DoJump);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Crouch"))), ETriggerEvent::Started, this, &ATPS_PlayerCharacter::Crouching);
		EnhancedInputComponent->BindAction(InputConfig->GetAction(FGameplayTag::RequestGameplayTag(FName("Input.Crouch"))), ETriggerEvent::Completed, this, &ATPS_PlayerCharacter::UnCrouching);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("---------- Can't Find EnhancedInputComponenet... ----------"))
	}
}

void ATPS_PlayerCharacter::Look(const FInputActionValue& Value)
{
	// 카메라 브로드캐스트
	if (CameraBoom)
	{
		OnLookInput.Broadcast(Value.Get<FVector3d>());
	}
}

void ATPS_PlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2d InputDirection = Value.Get<FVector2d>();

	FRotator CameraRotation = CameraBoom->GetComponentRotation();
	FVector CameraForward = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
	CameraForward.Z = 0;

	FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
	CameraRight.Z = 0;


	FVector MoveDirection = FVector((CameraForward * InputDirection.X) + (CameraRight * InputDirection.Y));
	MoveDirection.Normalize();
	AddMovementInput(MoveDirection);
}

void ATPS_PlayerCharacter::DoJump(const FInputActionValue& Value)
{
	TPSCharacterMoveComp->DoJump(true);
}

void ATPS_PlayerCharacter::Crouching(const FInputActionValue& Value)
{
	if (TPSCharacterMoveComp->IsMovingOnGround())
		ATPS_PlayerCharacter::Crouch();
	
}

void ATPS_PlayerCharacter::UnCrouching(const FInputActionValue& Value)
{
	ATPS_PlayerCharacter::UnCrouch();
}
