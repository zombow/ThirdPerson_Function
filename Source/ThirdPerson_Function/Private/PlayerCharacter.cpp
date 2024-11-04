// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
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

	// 플레이어 스켈레톤 설정
	PlayerSkeletalMeshComp = FindComponentByClass<USkeletalMeshComponent>();
	if (LoadObject<USkeletalMesh>(
		nullptr,TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'")))
	// TODO:load좌표를 다른곳에서 관리
	{
		PlayerSkeletalMeshComp->SetSkeletalMesh(LoadObject<USkeletalMesh>(
			nullptr,TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'")));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Find SkeletalMesh..."));
	}

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
	FVector2d MoveDirection = Value.Get<FVector2d>();

	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	AddMovementInput(ForwardVector, MoveDirection.X);
	AddMovementInput(RightVector, MoveDirection.Y);

	// 이동 방향이 있으면 캐릭터 회전
	if (!MoveDirection.IsNearlyZero())
	{
		FVector Direction = FVector{MoveDirection.Y, -MoveDirection.X, 0};
		// 이동 방향 벡터를 얻어 회전하도록 설정
		FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		FRotator NewRotation = FMath::RInterpTo(PlayerSkeletalMeshComp->GetRelativeRotation(), TargetRotation,
		                                        GetWorld()->GetDeltaSeconds(), 10);
		PlayerSkeletalMeshComp->SetRelativeRotation(NewRotation); // TODO: 카메라 방향을 기준으로 회전하게 변경필요
	}
}
