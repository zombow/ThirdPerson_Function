// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TPS_Player/TPS_PlayerController.h"
#include "TPS_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSON_FUNCTION_API UTPS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<ATPS_PlayerController> TPSPlayerController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bisPlayingMontage; // 상하체 블랜딩을 원할경우 활성화

	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UFUNCTION()
	void RotationInput(FVector2D Value);
	UFUNCTION(BlueprintNativeEvent)
	void Rotation(FVector2D  Value);
	virtual void Rotation_Implementation(FVector2D Value);
	
	
	UFUNCTION()
	void MoveOn(FInputActionInstance Value);
	UFUNCTION(BlueprintNativeEvent)
	void MoveOngoing(FInputActionInstance Value);
	virtual void MoveOngoing_Implementation(FInputActionInstance Value);

	UFUNCTION()
	void MoveEnd(FInputActionInstance Value);
	UFUNCTION(BlueprintNativeEvent)
	void MoveEnded(FInputActionInstance Value);
	virtual void MoveEnded_Implementation(FInputActionInstance Value);

};
