// Copyright (c) Bixboy, 2024. All Rights Reserved.
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
	void OnOpenLevelClicked(const FString& LevelPath);

#pragma endregion

// Menu
#pragma region Menu
private:
	void FillSubmenu(FMenuBuilder& MenuBuilder);
	void AddLevelToMenu(const FString& LevelName, FMenuBuilder& MenuBuilder);
	
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
void LoadCategoryOrderFromConfig(const FString& ConfigFilePath);

#pragma endregion

// Categories	
#pragma region Categories
private:
	TArray<FString> UnCategorizedLevels;
	TMap<FString, TArray<FString>> LevelCategories;
	TArray<FString> CategoryOrder;
	
	void MoveLevelToCategory(const FString& LevelName, const FString& TargetCategory);
	void OnDeleteCategory(const FString& CategoryName);
	void ReorderCategories(const FString& CategoryName, int32 NewIndex);
	void OnCreateNewCategory(const FString& LevelNameToMove);

#pragma endregion	
};
#endif