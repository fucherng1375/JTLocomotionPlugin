// Copyright Epic Games, Inc. All Rights Reserved.

#include "LocomotionPluginV2GameMode.h"
#include "LocomotionPluginV2Character.h"
#include "UObject/ConstructorHelpers.h"

ALocomotionPluginV2GameMode::ALocomotionPluginV2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
