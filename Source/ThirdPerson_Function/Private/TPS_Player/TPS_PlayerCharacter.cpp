// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerCharacter.h"

#include "NativeGameplayTags.h"
#include "TPS_Player/TPS_PlayerController.h"
#include "TPS_Player/TPS_PlayerState.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATPS_PlayerCharacter::ATPS_PlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTPS_CharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationYaw = false;

	// 카메라붐 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CameraBoom->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f), false);
	CameraBoom->SocketOffset = FVector(0, 70, 0);
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 50;
	CameraBoom->CameraRotationLagSpeed = 50;
	CameraBoom->CameraLagMaxDistance = 10;
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->bUsePawnControlRotation = true; // 카메라 리그로 캐릭터 회전

	// 카메라 설정
	PlayerCameraComp = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCameraComp->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	PlayerCameraComp->bUsePawnControlRotation = false;

	// 무브먼트 설정
	TPSCharacterMoveComp = FindComponentByClass<UTPS_CharacterMovementComponent>();
	TPSCharacterMoveComp->bUseControllerDesiredRotation = false; // 자유회전 사용
	TPSCharacterMoveComp->bOrientRotationToMovement = true; // 이동방향으로 캐릭터회전 활성화
	TPSCharacterMoveComp->NavAgentProps.bCanCrouch = true; // 앉기기능 활성화
	TPSCharacterMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	TPSCharacterMoveComp->RotationRate = FRotator(0.0f, -1.0f, 0.0f);
	TPSCharacterMoveComp->MaxWalkSpeed = 200.0f;
	TPSCharacterMoveComp->MaxWalkSpeedCrouched = 180.0f;
	PrimaryActorTick.bCanEverTick = true;

	// 무기수납 위치 설정
	WeaponSpot = CreateDefaultSubobject<USceneComponent>("Weapon Spot");
	WeaponSpot->SetupAttachment(PlayerSkeletalMeshComp, FName("spine_05"));

	// 인터렉션 컴포넌트
	TPSInteractionComponent = CreateDefaultSubobject<UTPS_InteractionComponent>("InteractionComp");

	// 인터렉션 컴포넌트에 사용할 BoxComponent
	TargetInteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TargetInteractionBox"));
	TargetInteractionBox->SetupAttachment(RootComponent);
	TargetInteractionBox->SetGenerateOverlapEvents(true);
	TargetInteractionBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	TPSInteractionComponent->InteractionBox = TargetInteractionBox;
}


// Called when the game starts or when spawned
void ATPS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 시작시 적용되는 Ability
	StaminaRegen(true);
}

#define DEFINE_INPUT_BINDING(Tag, FunctionName) \
InputDelegates.Find(FGameplayTag::RequestGameplayTag(Tag))->BindUObject(this, &ATPS_PlayerCharacter::FunctionName);

void ATPS_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Controller의 인풋바인딩
	if (TObjectPtr<ATPS_PlayerController> TPSController = Cast<ATPS_PlayerController>(Controller))
	{
		for (auto Pair : *TPSController->InputConfig->GetInputActionTagMap())
		{
			InputDelegates.Add(Pair.Key);
		}
		DEFINE_INPUT_BINDING("Input.Look", Look);
		DEFINE_INPUT_BINDING("Input.Move", Move);
		DEFINE_INPUT_BINDING("Input.Jump", DoJump);
		DEFINE_INPUT_BINDING("Input.Crouch", Crouching);
		DEFINE_INPUT_BINDING("Input.Roll", DoRoll);
		DEFINE_INPUT_BINDING("Input.Attack", Attack);
		DEFINE_INPUT_BINDING("Input.DrawWeapon", DrawWeapon);
		DEFINE_INPUT_BINDING("Input.DrawWeapon", SheathWeapon); // 중복허용을 안해 수정필요
		DEFINE_INPUT_BINDING("Input.Interaction", Interaction);
		if (!TPSController->OnInputs.IsAlreadyBound(this, &ATPS_PlayerCharacter::HandleInputs))
		{
			TPSController->OnInputs.AddDynamic(this, &ATPS_PlayerCharacter::HandleInputs);
		}
	}

	// ASC할당
	if (!TPSAbilitySystemComp)
	{
		TPSAbilitySystemComp = GetPlayerState<ATPS_PlayerState>()->GetAbilitySystemComponent();
	}

	TPSCharacterMoveComp->MovementModeChange.AddDynamic(this, &ATPS_PlayerCharacter::MovementModeChanged);
}

UAbilitySystemComponent* ATPS_PlayerCharacter::GetAbilitySystemComponent() const
{
	return TPSAbilitySystemComp;
}


TObjectPtr<UTPS_CharacterMovementComponent> ATPS_PlayerCharacter::GetTPSCharacterMovementComp() const
{
	return TPSCharacterMoveComp;
}


void ATPS_PlayerCharacter::HandleInputs(FInputActionInstance Instance, FGameplayTag Tag)
{
	if (auto Delegates = InputDelegates.Find(Tag))
	{
		// Tag에따라 Bind된 함수 실행
		Delegates->Execute(Instance);
	}
}

void ATPS_PlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	TPSAbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.InAir")));
	EndJump(nullptr);

	// LandVelocity 저장
	// LandVelocity를 기반으로 Landed 애니메이션을 재생할 수 있음
}

void ATPS_PlayerCharacter::MovementModeChanged(EMovementMode PreviousMovementMode, EMovementMode CurrentMovementMode, uint8 PreviousCustomMode)
{
	if (CurrentMovementMode == MOVE_Falling)
	{
		TPSAbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.InAir")));
	}
}

void ATPS_PlayerCharacter::Look(const FInputActionInstance& Value)
{
	AddControllerYawInput(Value.GetValue().Get<FVector2D>().X);
	AddControllerPitchInput(Value.GetValue().Get<FVector2D>().Y);
}

void ATPS_PlayerCharacter::Move(const FInputActionInstance& Value)
{
	FVector2D InputDirection = Value.GetValue().Get<FVector2D>();

	FVector InputX = UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0));
	FVector InputY = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0));

	DesiredDirection = FVector(InputDirection.Y, InputDirection.X, 0);

	AddMovementInput(InputX, InputDirection.X);
	AddMovementInput(InputY, InputDirection.Y);
}

void ATPS_PlayerCharacter::DoJump(const FInputActionInstance& Value)
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Jump")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Jump"));
	}
}

void ATPS_PlayerCharacter::EndJump(const FInputActionInstance& Value)
{
	FGameplayTagContainer JumpTagContainer = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Jump")));
	TPSAbilitySystemComp->CancelAbilities(&JumpTagContainer);
}

void ATPS_PlayerCharacter::Crouching(const FInputActionInstance& Value)
{
	if (Value.GetValue().Get<bool>())
	{
		if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Crouch")))))
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't Crouch"));
		}
	}
	else
	{
		FGameplayTagContainer CrouchTagContainer = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Crouch")));
		TPSAbilitySystemComp->CancelAbilities(&CrouchTagContainer);
	}
}

void ATPS_PlayerCharacter::DoRoll(const FInputActionInstance& Value)
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Roll")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Roll"));
	}
}

void ATPS_PlayerCharacter::Attack(const FInputActionInstance& Value)
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Attack"));
	}
}

void ATPS_PlayerCharacter::DrawWeapon(const FInputActionInstance& Value)
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't DrawWeapon"));
	}
}

void ATPS_PlayerCharacter::SheathWeapon(const FInputActionInstance& Value)
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.SheathWeapon")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SheathWeapon"));
	}
}

void ATPS_PlayerCharacter::Interaction(const FInputActionInstance& Value)
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Interaction")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Interaction"));
	}
}

void ATPS_PlayerCharacter::StaminaRegen(bool bActive)
{
	if (bActive)
	{
		TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.StaminaRegen"))));
	}
	else
	{
		TPSAbilitySystemComp->CancelAbility(
			TPSAbilitySystemComp->GetAbilitySpecFromTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.StaminaRegen")))->Ability);
	}
}

void ATPS_PlayerCharacter::AddLooseGameplayTag(FGameplayTag TagName)
{
	if (TPSAbilitySystemComp)
	{
		TPSAbilitySystemComp->AddLooseGameplayTag(TagName);
	}
}

void ATPS_PlayerCharacter::RemoveLooseGameplayTag(FGameplayTag TagName)
{
	if (TPSAbilitySystemComp)
	{
		TPSAbilitySystemComp->RemoveLooseGameplayTag(TagName);
	}
}
