// Copyright Epic Games, Inc. All Rights Reserved.
#if WITH_EDITOR
#include "LevelSelection.h"
#include "FileHelpers.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Engine/ObjectLibrary.h"

static const FName LevelSelectionTabName("LevelSelection");
#define LOCTEXT_NAMESPACE "FLevelSelectionModule"
DEFINE_LOG_CATEGORY(MyMenuLog)


#pragma region Setup Plugin

// Start Module
void FLevelSelectionModule::StartupModule()
{
	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
    
		MenuExtender->AddMenuBarExtension(
			"Help", // Positionner après le menu "Help"
			EExtensionHook::After, // Définir la position après le menu "Help"
			nullptr, // Aucune commande supplémentaire
			FMenuBarExtensionDelegate::CreateRaw(this, &FLevelSelectionModule::AddMenuEntry)
		);
		bIsTabRegistered = true;
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}	
}

//Close Module
void FLevelSelectionModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

TSharedRef<SDockTab> FLevelSelectionModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FLevelSelectionModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("LevelSelection.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FLevelSelectionModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(LevelSelectionTabName);
}

#pragma endregion

#pragma region Creat Menu

// Menu
void FLevelSelectionModule::AddMenuEntry(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
	  LOCTEXT("MenuLocKey", "Level Selection"),
	  LOCTEXT("MenuTooltipKey", "Opens menu for change level"),
	  FNewMenuDelegate::CreateRaw(this, &FLevelSelectionModule::FillSubmenu),
	  FName(TEXT("Level Selection")),
	  FName(TEXT("LevelSelection")));
}

// Sub Menu
void FLevelSelectionModule::FillSubmenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddWidget(
		SNew(STextBlock)
		.Text(LOCTEXT("LevelListTitle", "Available Levels"))
		.Justification(ETextJustify::Center),
		FText::GetEmpty()
	);

	// Ajouter un séparateur pour rendre le titre plus visible
	MenuBuilder.AddSeparator();
	
	TArray<FString> Levels = GetAllMapNames();

	if (Levels.Num() == 0)
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("NoLevels", "No levels available"),
			LOCTEXT("NoLevelsTooltip", "No levels were found to open."),
			FSlateIcon(),
			FUIAction()
		);
	}
	else
	{
		// Creation du Sub Menu
		for (const FString& LevelName : Levels)
		{
			FSlateIcon LevelIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.GameSettings");
			MenuBuilder.AddMenuEntry(
				FText::FromString(FPaths::GetBaseFilename(LevelName)),
				FText::FromString(FString::Printf(TEXT("Open level %s"), *FPaths::GetBaseFilename(LevelName))),
				LevelIcon,
				FUIAction(FExecuteAction::CreateLambda([this, LevelName]()
				{
					OnOpenLevelClicked(LevelName);
				}))
			);
		}
	}
}

#pragma endregion

#pragma region File Paths & Open Maps

// Find Levels
TArray<FString> FLevelSelectionModule::GetAllMapNames()
{
    auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game"));
	
    TArray<FAssetData> AssetDatas;
    ObjectLibrary->GetAssetDataList(AssetDatas);
    UE_LOG(LogTemp, Warning, TEXT("Found maps: %d"), AssetDatas.Num());

    TArray<FString> Names;

    for (int32 i = 0; i < AssetDatas.Num(); ++i)
    {
        FAssetData& AssetData = AssetDatas[i];

        FString FullPath = AssetData.GetSoftObjectPath().ToString();
        Names.Add(FullPath);
    }
	Names.Sort();
    return Names;
}

// Open Levels
void FLevelSelectionModule::OnOpenLevelClicked(const FString& LevelPath)
{
	// Vérifie s'il y a des modifications non sauvegardées
	if (FEditorFileUtils::SaveDirtyPackages(true, true, true))
	{
		FEditorFileUtils::LoadMap(LevelPath, false);
	}

}

#pragma endregion


#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FLevelSelectionModule, LevelSelection)

#endif