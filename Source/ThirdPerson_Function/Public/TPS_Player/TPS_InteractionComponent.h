// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "TPS_InteractionComponent.generated.h"


class ATPS_InteractableActor;
class ATPS_PlayerCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))

class THIRDPERSON_FUNCTION_API UTPS_InteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTPS_InteractionComponent();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "BoxComponent")
	TObjectPtr<UBoxComponent> InteractionBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "TPSPlayer")
	TObjectPtr<ATPS_PlayerCharacter> Player;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                    bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	ATPS_InteractableActor* FindNearActor(const FVector& Origin, const TSet<TObjectPtr<ATPS_InteractableActor>>& ActorSet);
	// Called when the game starts
	virtual void BeginPlay() override;
};
