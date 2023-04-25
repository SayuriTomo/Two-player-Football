// Copyright Epic Games, Inc. All Rights Reserved.

#include "A01_BlockoutSadiumGameMode.h"
#include "A01_BlockoutSadiumCharacter.h"
#include "UObject/ConstructorHelpers.h"

AA01_BlockoutSadiumGameMode::AA01_BlockoutSadiumGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
