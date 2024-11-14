// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelSelection.h"
#include "FileHelpers.h"
#include "LevelSelectionStyle.h"
#include "LevelSelectionCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Engine/ObjectLibrary.h"

static const FName LevelSelectionTabName("LevelSelection");

#define LOCTEXT_NAMESPACE "FLevelSelectionModule"
DEFINE_LOG_CATEGORY(MyMenuLog)

void FLevelSelectionModule::StartupModule()
{
    FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
    
    TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
    
    MenuExtender->AddMenuBarExtension(
        "Help", // Positionner après le menu "Help"
        EExtensionHook::After, // Définir la position après le menu "Help"
        nullptr, // Aucune commande supplémentaire
        FMenuBarExtensionDelegate::CreateRaw(this, &FLevelSelectionModule::AddMenuEntry)
    );
    
    LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FLevelSelectionModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FLevelSelectionStyle::Shutdown();
	FLevelSelectionCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LevelSelectionTabName);
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

void FLevelSelectionModule::AddMenuEntry(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
	  LOCTEXT("MenuLocKey", "Level Selection"),
	  LOCTEXT("MenuTooltipKey", "Opens menu for change level"),
	  FNewMenuDelegate::CreateRaw(this, &FLevelSelectionModule::FillSubmenu),
	  FName(TEXT("Level Selection")),
	  FName(TEXT("LevelSelection")));
}

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
		for (const FString& LevelName : Levels)
		{
			MenuBuilder.AddMenuEntry(
				FText::FromString(FPaths::GetBaseFilename(LevelName)),
				FText::FromString(FString::Printf(TEXT("Open level %s"), *FPaths::GetBaseFilename(LevelName))),
				FSlateIcon(),
				FUIAction(FExecuteAction::CreateLambda([this, LevelName]()
				{
					OnOpenLevelClicked(LevelName);
				}))
			);
		}
	}
}

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

void FLevelSelectionModule::OnOpenLevelClicked(const FString& LevelPath)
{
	// Ouvrir le niveau spécifié
	FEditorFileUtils::LoadMap(LevelPath, true);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLevelSelectionModule, LevelSelection)