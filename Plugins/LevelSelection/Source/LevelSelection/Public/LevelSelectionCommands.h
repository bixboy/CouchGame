#if WITH_EDITOR
#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "LevelSelectionStyle.h"

class FLevelSelectionCommands : public TCommands<FLevelSelectionCommands>
{
public:

	FLevelSelectionCommands()
		: TCommands<FLevelSelectionCommands>(TEXT("LevelSelection"), NSLOCTEXT("Contexts", "LevelSelection", "LevelSelection Plugin"), NAME_None, FLevelSelectionStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};

#endif