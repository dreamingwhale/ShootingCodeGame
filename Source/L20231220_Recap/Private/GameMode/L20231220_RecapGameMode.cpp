// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameMode/L20231220_RecapGameMode.h"
#include "GameMode/L20231220_RecapCharacter.h"
#include "UObject/ConstructorHelpers.h"

AL20231220_RecapGameMode::AL20231220_RecapGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
