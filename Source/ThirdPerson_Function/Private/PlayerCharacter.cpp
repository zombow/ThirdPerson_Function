// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
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
	CharacterMovementComp = FindComponentByClass<UCharacterMovementComponent>();
	CharacterMovementComp->bUseControllerDesiredRotation = true; // 컨트롤 회전값으로 캐릭터회전 활성화
	CharacterMovementComp->bOrientRotationToMovement = true; // 이동방향으로 캐릭터회전 활성화
	CharacterMovementComp->NavAgentProps.bCanCrouch = true; // 앉기기능 활성화
	CharacterMovementComp->RotationRate = FRotator(0.0f, 460.0f, 0.0f);
	CharacterMovementComp->MaxWalkSpeed = 600.0f;
	CharacterMovementComp->MaxWalkSpeedCrouched = 300.0f;

	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Enhanced Input 시스템 설정 (PlayerController에서 Enhanced Input Subsystem 사용)
	if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller))
	{
		if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMCTPSCharacter, 0);
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT(" Current Speed: %hhd"), CharacterMovementComp->IsCrouching());
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input 컴포넌트로 캐스팅
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<
		UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(IACrouch, ETriggerEvent::Started, this, &APlayerCharacter::Crouch);
		EnhancedInputComponent->BindAction(IACrouch, ETriggerEvent::Completed, this, &APlayerCharacter::UnCrouch);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("---------- Can't Find EnhancedInputComponenet... ----------"))
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// 카메라 브로드캐스트
	if (CameraBoom)
	{
		OnLookInput.Broadcast(Value.Get<FVector3d>());
	}
}

void APlayerCharacter::Move(const FInputActionValue& Value)
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

void APlayerCharacter::Jump(const FInputActionValue& Value)
{
	CharacterMovementComp->DoJump(true);
}

void APlayerCharacter::Crouch(const FInputActionValue& Value)
{
	CharacterMovementComp->Crouch();
	UE_LOG(LogTemp, Warning, TEXT("CROUCH! / %hhd / Current Speed: %f"), CharacterMovementComp->IsCrouching(),
	       CharacterMovementComp->MaxWalkSpeedCrouched);
}

void APlayerCharacter::UnCrouch(const FInputActionValue& Value)
{
	CharacterMovementComp->UnCrouch();
	UE_LOG(LogTemp, Warning, TEXT("UnCROUCH! / %hhd / Current Speed: %f"), CharacterMovementComp->IsCrouching(),
	       CharacterMovementComp->MaxWalkSpeed);
}
