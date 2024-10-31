// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PlayerSkeletalMeshComp = FindComponentByClass<USkeletalMeshComponent>();
	if (LoadObject<USkeletalMesh>(nullptr,TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"))) // TODO:load좌표를 다른곳에서 관리
	{
		PlayerSkeletalMeshComp->SetSkeletalMesh(LoadObject<USkeletalMesh>(nullptr,TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'")));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Find SkeletalMesh..."));
	}
	
	PlayerCameraComp = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCameraComp->SetupAttachment(RootComponent);
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
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
}
