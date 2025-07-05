// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryPluginGameMode.h"
#include "InventoryPluginCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventoryPluginGameMode::AInventoryPluginGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
