// Copyright Epic Games, Inc. All Rights Reserved.
#if WITH_EDITOR
#include "LevelSelectionCommands.h"

#define LOCTEXT_NAMESPACE "FLevelSelectionModule"

void FLevelSelectionCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "LevelSelection", "Bring up LevelSelection window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
#endif