// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/CouchGameModeMenu.h"
#include "CouchGame/Public/Arena//CouchPlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CouchCharacterInputData.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSubsystem.h"
#include "Arena/CouchGameManagerSubSystem.h"
#include "Characters/CouchCharacterSettings.h"
#include "CharactersMenu/CouchCharacterMenu.h"



void ACouchGameModeMenu::BeginPlay()
{
	Super::BeginPlay();
	CreateAndInitPlayers();
	TArray<ACouchPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacter(PlayerStartsPoints);
	}

UCouchCharacterInputData* ACouchGameModeMenu::LoadInputDataFromConfig()
{
	const UCouchCharacterSettings* CharacterSettings = GetDefault<UCouchCharacterSettings>();
	if (!CharacterSettings) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* ACouchGameModeMenu::LoadInputMappingContextFromConfig()
{
	const UCouchCharacterSettings* CharacterSettings = GetDefault<UCouchCharacterSettings>();
	if (!CharacterSettings) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
}

void ACouchGameModeMenu::FindPlayerStartActorsInArena(TArray<ACouchPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		ACouchPlayerStart* PlayerStartActor = Cast<ACouchPlayerStart>(FoundActors[i]);
		if (!PlayerStartActor) continue;
		
		ResultsActors.Add(PlayerStartActor);
	}
}

void ACouchGameModeMenu::SpawnCharacter(const TArray<ACouchPlayerStart*>& SpawnPoints)
{
	if (SpawnPoints.Num() == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "No Spawn Point");
		return;
	};
	UCouchCharacterInputData* InputData = LoadInputDataFromConfig();
	// UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	
	// UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	for	(ACouchPlayerStart* SpawnPoint : SpawnPoints)
	{
		if (!SpawnPoint) return;
		ACouchCharacterMenu* NewCharacter = GetWorld()->SpawnActorDeferred<ACouchCharacterMenu>(MenuCharacter,
			SpawnPoint->GetTransform());
		if (!NewCharacter)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Failed to Spawn Character");
			continue;
		}
		NewCharacter->InputData = InputData;
		// NewCharacter->InputMappingContext = InputMappingContext;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->AutoReceiveInput = SpawnPoint->AutoReceiveInput;
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
		
		CharactersInMenu.Add(NewCharacter);
	}
}

void ACouchGameModeMenu::CreateAndInitPlayers() const
{
	const UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (!LocalMultiplayerSubsystem) return;
	const UCouchGameManagerSubSystem* GameManager = GameInstance->GetSubsystem<UCouchGameManagerSubSystem>();
	if (!GameManager) return;
	LocalMultiplayerSubsystem->CreateAndInitPlayers(GameManager->MappingType);
}
