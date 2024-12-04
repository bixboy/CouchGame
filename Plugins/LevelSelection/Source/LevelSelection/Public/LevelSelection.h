#pragma once
#if WITH_EDITOR
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"

class FToolBarBuilder;
class FMenuBuilder;

DECLARE_LOG_CATEGORY_EXTERN(MyMenuLog, Log, All);

class FLevelSelectionModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:
	
	void AddMenuEntry(FMenuBarBuilder& MenuBuilder);
	void FillSubmenu(FMenuBuilder& MenuBuilder);
	void OnOpenLevelClicked(const FString& LevelPath);

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

private:
	TSharedPtr<class FUICommandList> PluginCommands;

	bool bIsTabRegistered = false;

#pragma region Find Path
private:
	TArray<FString> GetAllMapNames();

#pragma endregion

#pragma region Copy Name
private:
	void OnCopyLevelNameClicked(const FString& LevelName);

#pragma endregion
	
};
#endif