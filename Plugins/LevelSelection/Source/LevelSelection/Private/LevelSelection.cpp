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
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

static const FName LevelSelectionTabName("LevelSelection");
#define LOCTEXT_NAMESPACE "FLevelSelectionModule"
DEFINE_LOG_CATEGORY(MyMenuLog)

// Setup
#pragma region Setup Plugin

// Démarrage du module
void FLevelSelectionModule::StartupModule()
{
	LoadCategoriesFromConfig();
	if (FModuleManager::Get().IsModuleLoaded("LevelEditor"))
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());

		MenuExtender->AddMenuBarExtension(
			"Help", // Positionner après le menu "Help"
			EExtensionHook::After,
			nullptr,
			FMenuBarExtensionDelegate::CreateRaw(this, &FLevelSelectionModule::AddMenuEntry)
		);
		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
}

// Fermeture du module
void FLevelSelectionModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

TSharedRef<SDockTab> FLevelSelectionModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("WindowWidgetText", "Welcome to Level Selection Plugin"))
			]
		];
}

void FLevelSelectionModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(LevelSelectionTabName);
}

#pragma endregion

// Création Menu
#pragma region Menu Creation

void FLevelSelectionModule::AddMenuEntry(FMenuBarBuilder& MenuBuilder)
{
	MenuBuilder.AddPullDownMenu(
		LOCTEXT("MenuLocKey", "Level Selection"),
		LOCTEXT("MenuTooltipKey", "Open the Level Selection menu"),
		FNewMenuDelegate::CreateRaw(this, &FLevelSelectionModule::FillSubmenu),
		FName(TEXT("Level Selection"))
	);
}

void FLevelSelectionModule::FillSubmenu(FMenuBuilder& MenuBuilder)
{
	if (LevelCategories.Num() > 0)
	{
		for (const auto& CategoryPair : LevelCategories)
		{
			const FString& CategoryName = CategoryPair.Key;
			const TArray<FString>& Levels = CategoryPair.Value;

			MenuBuilder.AddSubMenu(
				FText::FromString(CategoryName),
				LOCTEXT("CategoryTooltip", "Options for this category"),
				FNewMenuDelegate::CreateLambda([this, Levels](FMenuBuilder& SubMenuBuilder)
				{
					if (Levels.Num() > 0)
					{
						for (const FString& LevelName : Levels)
						{
							AddLevelToMenu(LevelName, SubMenuBuilder);
						}
					}
					else
					{
						SubMenuBuilder.AddMenuEntry(
							LOCTEXT("EmptyCategory", "No levels in this category."),
							FText::GetEmpty(),
							FSlateIcon(),
							FUIAction()
						);
					}
				})
			);
		}
	}

	// Ajouter une catégorie pour les niveaux non catégorisés
	TArray<FString> UnCategorizedLevels = GetAllMapNames();

	for (const auto& CategoryPair : LevelCategories)
	{
		for (const FString& LevelName : CategoryPair.Value)
		{
			UnCategorizedLevels.Remove(LevelName);
		}
	}

	MenuBuilder.AddSubMenu(
		LOCTEXT("Uncategorized", "Uncategorized Levels"),
		LOCTEXT("UncategorizedTooltip", "Levels that are not in any category."),
		FNewMenuDelegate::CreateLambda([this, UnCategorizedLevels](FMenuBuilder& SubMenuBuilder)
		{
			if (UnCategorizedLevels.Num() > 0)
			{
				for (const FString& LevelName : UnCategorizedLevels)
				{
					AddLevelToMenu(LevelName, SubMenuBuilder);
				}
			}
			else
			{
				SubMenuBuilder.AddMenuEntry(
					LOCTEXT("NoUncategorizedLevels", "No uncategorized levels found."),
					FText::GetEmpty(),
					FSlateIcon(),
					FUIAction()
				);
			}
		})
	);
}

void FLevelSelectionModule::AddLevelToMenu(const FString& LevelName, FMenuBuilder& MenuBuilder)
{
    MenuBuilder.BeginSection("LevelSection", LOCTEXT("LevelSection", "Level"));

    MenuBuilder.AddMenuEntry(
        FText::FromString(FPaths::GetBaseFilename(LevelName)),
        FText::FromString(FString::Printf(TEXT("Open level: %s"), *LevelName)),
        FSlateIcon(),
        FUIAction(FExecuteAction::CreateLambda([this, LevelName]()
        {
            OnOpenLevelClicked(LevelName);
        }))
    );

    // Options 1 : Copy Name Level
    MenuBuilder.AddSubMenu(
        LOCTEXT("Options", "Options"),
        LOCTEXT("OptionsTooltip", "Hover over to show options."),
        FNewMenuDelegate::CreateLambda([this, LevelName](FMenuBuilder& SubMenuBuilder)
        {
            SubMenuBuilder.AddMenuEntry(
                LOCTEXT("CopyLevelName", "Copy Level Name"),
                LOCTEXT("CopyLevelNameTooltip", "Copy the name of this level to clipboard."),
                FSlateIcon(),
                FUIAction(FExecuteAction::CreateLambda([this, LevelName]()
                {
                    OnCopyLevelNameClicked(LevelName);
                }))
            );

            // Option 2 : Déplacer vers une catégorie
            SubMenuBuilder.AddSubMenu(
                LOCTEXT("MoveToCategory", "Move To Category"),
                LOCTEXT("MoveToCategoryTooltip", "Move this level to another category."),
                FNewMenuDelegate::CreateLambda([this, LevelName](FMenuBuilder& MoveMenuBuilder)
                {
                    // Trouver la catégorie actuelle du niveau
                    FString CurrentCategory;
                    for (const auto& CategoryPair : LevelCategories)
                    {
                        if (CategoryPair.Value.Contains(LevelName))
                        {
                            CurrentCategory = CategoryPair.Key;
                            break;
                        }
                    }

                    MoveMenuBuilder.AddMenuEntry(
                        LOCTEXT("CreateNewCategory", "Create New Category"),
                        LOCTEXT("CreateNewCategoryTooltip", "Add a new category to organize your levels."),
                        FSlateIcon(),
                        FUIAction(FExecuteAction::CreateLambda([this, LevelName]()
                        {
                            OnCreateNewCategory(LevelName);
                        }))
                    );

                    MoveMenuBuilder.AddSeparator();

                    for (const auto& CategoryPair : LevelCategories)
                    {
                        const FString& TargetCategory = CategoryPair.Key;

                        // Ne pas afficher la catégorie actuelle
                        if (TargetCategory == CurrentCategory)
                        {
                            continue;
                        }

                        MoveMenuBuilder.AddMenuEntry(
                            FText::FromString(TargetCategory),
                            FText::FromString(FString::Printf(TEXT("Move %s to category %s"), *FPaths::GetBaseFilename(LevelName), *TargetCategory)),
                            FSlateIcon(),
                            FUIAction(FExecuteAction::CreateLambda([this, LevelName, TargetCategory]()
                            {
                                MoveLevelToCategory(LevelName, TargetCategory);
                            }))
                        );
                    }
                })
            );
        })
    );

    MenuBuilder.EndSection();
}

#pragma endregion

// Levels Management
#pragma region Level Management

TArray<FString> FLevelSelectionModule::GetAllMapNames()
{
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game"));

	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	TArray<FString> Names;

	for (const FAssetData& AssetData : AssetDatas)
	{
		Names.Add(AssetData.GetSoftObjectPath().ToString());
	}

	Names.Sort();
	return Names;
}

void FLevelSelectionModule::OnOpenLevelClicked(const FString& LevelPath)
{
	if (FEditorFileUtils::SaveDirtyPackages(true, true, true))
	{
		FEditorFileUtils::LoadMap(LevelPath, false);
	}
}

#pragma endregion

// Categories
#pragma region Categories Management

void FLevelSelectionModule::OnCreateNewCategory(const FString& LevelNameToMove)
{
    TSharedRef<SWindow> InputWindow = SNew(SWindow)
        .Title(FText::FromString("Create New Category"))
        .ClientSize(FVector2D(400, 200))
        .SupportsMinimize(false)
        .SupportsMaximize(false);

    TSharedPtr<SEditableTextBox> InputTextBox;
    InputWindow->SetContent(
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .Padding(10)
        .HAlign(HAlign_Fill)
        [
            SNew(STextBlock)
            .Text(FText::FromString("Enter the new category name:"))
        ]
        + SVerticalBox::Slot()
        .Padding(10)
        .HAlign(HAlign_Fill)
        [
            SAssignNew(InputTextBox, SEditableTextBox)
            .HintText(FText::FromString("Category Name"))
        ]
        + SVerticalBox::Slot()
        .Padding(10)
        .HAlign(HAlign_Center)
        [
            SNew(SButton)
            .Text(FText::FromString("OK"))
            .OnClicked_Lambda([InputWindow, InputTextBox, this, LevelNameToMove]() -> FReply
            {
                if (!InputTextBox.IsValid())
                {
                    UE_LOG(LogTemp, Error, TEXT("InputTextBox is not valid."));
                    return FReply::Unhandled();
                }

                const FString NewCategoryName = InputTextBox->GetText().ToString().TrimStartAndEnd();

                if (NewCategoryName.IsEmpty())
                {
                    UE_LOG(LogTemp, Warning, TEXT("Category name is empty."));
                    return FReply::Handled();
                }

                if (LevelCategories.Contains(NewCategoryName))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Category already exists: %s"), *NewCategoryName);
                    return FReply::Handled();
                }

                LevelCategories.Add(NewCategoryName);
                UE_LOG(LogTemp, Log, TEXT("Created new category: %s"), *NewCategoryName);

                if (!LevelNameToMove.IsEmpty())
                {
                    MoveLevelToCategory(LevelNameToMove, NewCategoryName);
                }

                InputWindow->RequestDestroyWindow();
                return FReply::Handled();
            })
        ]
    );

    FSlateApplication::Get().AddWindow(InputWindow);
}

void FLevelSelectionModule::MoveLevelToCategory(const FString& LevelName, const FString& TargetCategory)
{
	bool bLevelMoved = false;
    bool bLevelFound = false;

    if (LevelName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Level name is empty."));
        ShowTemporaryNotification(LOCTEXT("LevelNotFound", "Level name is empty."));
        return;
    }

    for (auto& CategoryPair : LevelCategories)
    {
        int32 RemovedCount = CategoryPair.Value.Remove(LevelName);
        if (RemovedCount > 0)
        {
            LevelCategories.FindOrAdd(TargetCategory).AddUnique(LevelName);

            ShowTemporaryNotification(FText::Format(
                LOCTEXT("LevelMoved", "Level '{0}' moved to category '{1}'!"),
                FText::FromString(LevelName),
                FText::FromString(TargetCategory)
            ));

            bLevelMoved = true;
            bLevelFound = true;
            break;
        }
    }

    if (!bLevelFound)
    {
        LevelCategories.FindOrAdd(TargetCategory).AddUnique(LevelName);

        ShowTemporaryNotification(FText::Format(
            LOCTEXT("LevelMoved", "Level '{0}' moved to category '{1}'!"),
            FText::FromString(LevelName),
            FText::FromString(TargetCategory)
        ));

        bLevelMoved = true;
    }

    if (bLevelMoved)
    {
        TArray<FString> CategoriesToDelete;

        for (auto& CategoryPair : LevelCategories)
        {
            if (CategoryPair.Value.Num() == 0)
            {
                CategoriesToDelete.Add(CategoryPair.Key);
            }
        }

        for (const FString& CategoryToDelete : CategoriesToDelete)
        {
            OnDeleteCategory(CategoryToDelete);
        }
    }
	SaveCategoriesToConfig();
	
    if (!bLevelMoved)
    {
        ShowTemporaryNotification(LOCTEXT("LevelNotFound", "Level not found in any category."));
    }
}

void FLevelSelectionModule::OnDeleteCategory(const FString& CategoryName)
{
    if (!CategoryName.IsEmpty() && LevelCategories.Contains(CategoryName))
    {
        LevelCategories.Remove(CategoryName);
        ShowTemporaryNotification(FText::Format(LOCTEXT("CategoryDeleted", "Category '{0}' deleted!"), FText::FromString(CategoryName)));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Category does not exist: %s"), *CategoryName);
        ShowTemporaryNotification(FText::Format(LOCTEXT("CategoryNotFound", "Category '{0}' not found."), FText::FromString(CategoryName)));
    }
}

#pragma endregion

// Copy Name
#pragma region Copy Name
	
void FLevelSelectionModule::OnCopyLevelNameClicked(const FString& LevelName)
{
	FPlatformApplicationMisc::ClipboardCopy(*FPaths::GetBaseFilename(LevelName));
	UE_LOG(LogTemp, Log, TEXT("Copied Level Name: %s"), *FPaths::GetBaseFilename(LevelName));

	// Message temporaire
	ShowTemporaryNotification(FText::Format(
		LOCTEXT("CopiedNotification", "Level Name '{0}' Copied!"),
		FText::FromString(FPaths::GetBaseFilename(LevelName))
	));
}

#pragma endregion

//Notifications
#pragma region Notifications

void FLevelSelectionModule::ShowTemporaryNotification(const FText& NotificationText)
{
	FNotificationInfo Info(NotificationText);
	Info.ExpireDuration = 3.0f;
	FSlateNotificationManager::Get().AddNotification(Info)->SetCompletionState(SNotificationItem::CS_Success);
}

#pragma endregion

// Saving
#pragma region Save

void FLevelSelectionModule::SaveCategoriesToConfig()
{
    // Récupère le chemin
    FString ConfigFilePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectConfigDir() + TEXT("LevelCategories.ini"));
    FFileHelper::SaveStringToFile(TEXT(""), *ConfigFilePath);

    // Parcourt les sections existantes dans le fichier pour :
    // 1. Supprimer les catégories obsolètes.
    // 2. Nettoyer les niveaux déplacés.
	TSet<FString> CurrentCategories;
	TMap<FString, TSet<FString>> CurrentLevelsInCategories;

	for (const auto& CategoryPair : LevelCategories)
	{
		const FString& CategoryName = CategoryPair.Key;
		const TArray<FString>& Levels = CategoryPair.Value;

		CurrentCategories.Add(CategoryName);
		
		TSet<FString> LevelsSet(Levels);
		CurrentLevelsInCategories.Add(CategoryName, LevelsSet);
	}

	TArray<FString> ExistingCategories;
	GConfig->GetSectionNames(ConfigFilePath, ExistingCategories);

	for (const FString& ExistingCategory : ExistingCategories)
	{
		// Vérifier si la catégorie existe encore
		if (!CurrentCategories.Contains(ExistingCategory))
		{
			UE_LOG(LogTemp, Log, TEXT("Deleting category '%s' from config"), *ExistingCategory);
			GConfig->EmptySection(*ExistingCategory, ConfigFilePath);
			continue;
		}

		// Supprimer les niveaux obsolètes
		TArray<FString> StoredLevels;
		if (GConfig->GetSection(*ExistingCategory, StoredLevels, ConfigFilePath))
		{
			const TSet<FString>* CurrentLevels = CurrentLevelsInCategories.Find(ExistingCategory);
			if (CurrentLevels)
			{
				for (const FString& StoredLevel : StoredLevels)
				{
					if (!CurrentLevels->Contains(StoredLevel))
					{
						GConfig->EmptySection(*ExistingCategory, ConfigFilePath);
						GConfig->RemoveKey(*ExistingCategory, *StoredLevel, ConfigFilePath);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve levels for category '%s'"), *ExistingCategory);
		}
	}
	GConfig->Flush(false, ConfigFilePath);
	
    // Sauvegarde les catégories et leurs niveaux
	for (const auto& CategoryPair : LevelCategories)
	{
		const FString& CategoryName = CategoryPair.Key;
		const TArray<FString>& LevelNames = CategoryPair.Value;

		UE_LOG(LogTemp, Log, TEXT("Saving category: %s"), *CategoryName);

		for (const FString& LevelName : LevelNames)
		{
			UE_LOG(LogTemp, Log, TEXT("  Level: %s"), *LevelName);
			GConfig->SetString(*CategoryName, *LevelName, *LevelName, ConfigFilePath);
		}
	}

    GConfig->Flush(false, ConfigFilePath);
    UE_LOG(LogTemp, Log, TEXT("Level categories successfully saved to: %s"), *ConfigFilePath);
}

void FLevelSelectionModule::LoadCategoriesFromConfig()
{
	FString CategoriesConfigFile = FPaths::ProjectConfigDir() + "LevelCategories.ini";
	if (FPaths::FileExists(CategoriesConfigFile))
	{
		TArray<FString> CategoryNames;
		if (GConfig->GetSectionNames(CategoriesConfigFile, CategoryNames))
		{
			for (const FString& CategoryName : CategoryNames)
			{
				TArray<FString> LevelEntries;
				if (GConfig->GetSection(*CategoryName, LevelEntries, CategoriesConfigFile))
				{
					for (const FString& LevelEntry : LevelEntries)
					{
						// Chaque entrée a le format "Key=Value", donc on les sépare
						FString LevelKey, LevelValue;
						if (LevelEntry.Split(TEXT("="), &LevelKey, &LevelValue))
						{
							LevelCategories.FindOrAdd(CategoryName).Add(LevelValue);
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to load levels for category: %s"), *CategoryName);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No categories found in config file: %s"), *CategoriesConfigFile);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Config file does not exist: %s"), *CategoriesConfigFile);
	}
}

#pragma endregion

#undef LOCTEXT_NAMESPACE
IMPLEMENT_MODULE(FLevelSelectionModule, LevelSelection)

#endif