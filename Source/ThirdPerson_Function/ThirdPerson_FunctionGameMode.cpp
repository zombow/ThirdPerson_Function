// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPerson_FunctionGameMode.h"
#include "ThirdPerson_FunctionCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPerson_FunctionGameMode::AThirdPerson_FunctionGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
