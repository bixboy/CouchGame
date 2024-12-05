#pragma once
#if WITH_EDITOR

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"

class FToolBarBuilder;
class FMenuBuilder;

DECLARE_LOG_CATEGORY_EXTERN(MyMenuLog, Log, All);

class LEVELSELECTION_API FLevelSelectionModule : public IModuleInterface
{
// Setup	
#pragma region Setup	
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void PluginButtonClicked();
	
private:
	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	
	void AddMenuEntry(FMenuBarBuilder& MenuBuilder);
	void FillSubmenu(FMenuBuilder& MenuBuilder);
void AddLevelToMenu(const FString& LevelName, FMenuBuilder& MenuBuilder);
void OnOpenLevelClicked(const FString& LevelPath);

#pragma endregion	

// Find Level Path	
#pragma region Find Path
private:
	TArray<FString> GetAllMapNames();

#pragma endregion

// Copy Level Name	
#pragma region Copy Name
private:
	void OnCopyLevelNameClicked(const FString& LevelName);
	void ShowTemporaryNotification(const FText& NotificationText);
void SaveCategoriesToConfig();
void LoadCategoriesFromConfig();

#pragma endregion

// Categories	
#pragma region Categories
private:
	TMap<FString, TArray<FString>> LevelCategories;

	void MoveLevelToCategory(const FString& LevelName, const FString& TargetCategory);
	void OnDeleteCategory(const FString& CategoryName);
	void OnCreateNewCategory();

#pragma endregion	
};
#endif