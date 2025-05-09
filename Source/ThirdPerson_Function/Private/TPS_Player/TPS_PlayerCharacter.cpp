// Fill out your copyright notice in the Description page of Project Settings.


#include "TPS_Player/TPS_PlayerCharacter.h"

#include "NativeGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/BlueprintTypeConversions.h"
#include "TPS_Player/TPS_PlayerController.h"
#include "TPS_Player/TPS_PlayerState.h"
#include "Kismet/KismetMathLibrary.h"

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
	TPSCharacterMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	TPSCharacterMoveComp->RotationRate = FRotator(0.0f, 460.0f, 0.0f);
	TPSCharacterMoveComp->MaxWalkSpeed = 300.0f;
	TPSCharacterMoveComp->MaxWalkSpeedCrouched = 250.0f;
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

	if (TObjectPtr<ATPS_PlayerController> TPSController = Cast<ATPS_PlayerController>(Controller))
	{
		// Controller의 인풋바인딩
		TPSController->OnRotationInput.AddDynamic(this, &ATPS_PlayerCharacter::Rotation);
		TPSController->OnMoveOngoing.AddDynamic(this, &ATPS_PlayerCharacter::MoveOnGoing);
		TPSController->OnJumpInput.AddDynamic(this, &ATPS_PlayerCharacter::DoJump);
		TPSController->OnCrouchingInput.AddDynamic(this, &ATPS_PlayerCharacter::Crouching);
		TPSController->OnRollInput.AddDynamic(this, &ATPS_PlayerCharacter::DoRoll);
		TPSController->OnAttackInput.AddDynamic(this, &ATPS_PlayerCharacter::Attack);
		TPSController->OnDrawWeapon.AddDynamic(this, &ATPS_PlayerCharacter::DrawWeapon);
		TPSController->OnDrawWeapon.AddDynamic(this, &ATPS_PlayerCharacter::SheathWeapon);
		TPSController->OnInteraction.AddDynamic(this, &ATPS_PlayerCharacter::Interaction);

		TPSCharacterMoveComp->MovementModeChange.AddDynamic(this, &ATPS_PlayerCharacter::MovementModeChanged);
	}

	// 시작시 적용되는 Ability
	StaminaRegen(true);
}

void ATPS_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// ASC할당
	if (!TPSAbilitySystemComp)
	{
		TPSAbilitySystemComp = GetPlayerState<ATPS_PlayerState>()->GetAbilitySystemComponent();
		TPSAbilitySystemComp->InitAbilityActorInfo(GetPlayerState<ATPS_PlayerState>(), this);
		auto TPSAbilitySet = TPSAbilities->GetTPSAbilities();

		// Ability 바인딩
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.Jump"))], FGameplayTag::RequestGameplayTag(TEXT("Ability.Jump")), 1);
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.Roll"))], FGameplayTag::RequestGameplayTag(TEXT("Ability.Roll")), 1);
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.Crouch"))], FGameplayTag::RequestGameplayTag(TEXT("Ability.Crouch")),
		            1);
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack"))], FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack")),
		            1);
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon"))],
		            FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")), 1);
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.SheathWeapon"))],
		            FGameplayTag::RequestGameplayTag(TEXT("Ability.SheathWeapon")), 1);
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.StaminaRegen"))],
		            FGameplayTag::RequestGameplayTag(TEXT("Ability.StaminaRegen")), 1);
		AbilityBind(TPSAbilitySet[FGameplayTag::RequestGameplayTag(TEXT("Ability.Interaction"))],
		            FGameplayTag::RequestGameplayTag(TEXT("Ability.Interaction")), 1);
	}
}

UAbilitySystemComponent* ATPS_PlayerCharacter::GetAbilitySystemComponent() const
{
	return TPSAbilitySystemComp;
}

FVector ATPS_PlayerCharacter::GetTPSLastInput()
{
	return TPSLastInput;
}

FGameplayAbilitySpec* ATPS_PlayerCharacter::GetAbilitySpec(FGameplayTag AbilityTag)
{
	if (AbilitySpecs.Find(AbilityTag))
	{
		return &AbilitySpecs[AbilityTag];
	}
	return nullptr;
}

TObjectPtr<UTPS_CharacterMovementComponent> ATPS_PlayerCharacter::GetTPSCharacterMovementComp() const
{
	return TPSCharacterMoveComp;
}


void ATPS_PlayerCharacter::AbilityBind(TSubclassOf<UGameplayAbility>& AbilityClass, FGameplayTag AbilityTag, int Level)
{
	FGameplayAbilitySpec Temp(AbilityClass, Level);
	AbilitySpecs.Add(AbilityTag, Temp);
	TPSAbilitySystemComp->GiveAbility(AbilitySpecs[AbilityTag]);
}

void ATPS_PlayerCharacter::MovementModeChanged(EMovementMode PreviousMovementMode, EMovementMode CurrentMovementMode, uint8 PreviousCustomMode)
{
	if (CurrentMovementMode == MOVE_Falling)
	{
		TPSAbilitySystemComp->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.InAir")));
	}
	else if ((PreviousMovementMode == MOVE_Falling) && (CurrentMovementMode != MOVE_Falling))
	{
		TPSAbilitySystemComp->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("State.Character.InAir")));
		EndJump();
	}
}

void ATPS_PlayerCharacter::Rotation(FVector2D Value)
{
	FVector2D InputDirection = Value;
	RotationAndMove(InputDirection);
}

void ATPS_PlayerCharacter::MoveOnGoing(FInputActionInstance Value)
{
	FVector2D InputDirection = Value.GetValue().Get<FVector2D>();
	RotationAndMove(InputDirection);
	AddMovementInput(DesiredDirection);
}

void ATPS_PlayerCharacter::DoJump()
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Jump")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Jump"));
	}
}

void ATPS_PlayerCharacter::EndJump()
{
	FGameplayTagContainer JumpTagContainer = FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Jump")));
	TPSAbilitySystemComp->CancelAbilities(&JumpTagContainer);
}

void ATPS_PlayerCharacter::Crouching(bool bCrouch)
{
	if (bCrouch)
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

void ATPS_PlayerCharacter::DoRoll()
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Roll")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Roll"));
	}
}

void ATPS_PlayerCharacter::Attack()
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Attack"));
	}
}

void ATPS_PlayerCharacter::DrawWeapon()
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.DrawWeapon")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't DrawWeapon"));
	}
}

void ATPS_PlayerCharacter::SheathWeapon()
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.SheathWeapon")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SheathWeapon"));
	}
}

void ATPS_PlayerCharacter::Interaction()
{
	if (!TPSAbilitySystemComp->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability.Interaction")))))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Interaction"));
	}
}

void ATPS_PlayerCharacter::RotationAndMove(FVector2D InputValue2D)
{
	FRotator CameraRotation = CameraBoom->GetComponentRotation();
	FVector CameraForward = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
	CameraForward.Z = 0;
	FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
	CameraRight.Z = 0;

	DesiredDirection = (CameraForward * InputValue2D.X) + (CameraRight * InputValue2D.Y);
	DesiredDirection.Normalize();

	if (InputValue2D != FVector2D::ZeroVector)
	{
		TPSLastInput = DesiredDirection;
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
		TPSAbilitySystemComp->CancelAbility(AbilitySpecs.Find(FGameplayTag::RequestGameplayTag(TEXT("Ability.StaminaRegen")))->Ability);
	}
}
