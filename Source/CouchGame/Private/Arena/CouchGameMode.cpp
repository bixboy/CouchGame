// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/CouchGameMode.h"

#include "Arena/ArenaSettings.h"
#include "CouchGame/Public/Arena//CouchPlayerStart.h"
#include "CouchGame/Public/Characters/CouchCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CouchCharacterInputData.h"
#include "InputMappingContext.h"
#include "Characters/CouchCharacterSettings.h"


void ACouchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<ACouchPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacter(PlayerStartsPoints);
}

UCouchCharacterInputData* ACouchGameMode::LoadInputDataFromConfig()
{
	const UCouchCharacterSettings* CharacterSettings = GetDefault<UCouchCharacterSettings>();
	if (!CharacterSettings) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* ACouchGameMode::LoadInputMappingContextFromConfig()
{
	const UCouchCharacterSettings* CharacterSettings = GetDefault<UCouchCharacterSettings>();
	if (!CharacterSettings) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
}

void ACouchGameMode::FindPlayerStartActorsInArena(TArray<ACouchPlayerStart*>& ResultsActors)
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

void ACouchGameMode::SpawnCharacter(const TArray<ACouchPlayerStart*>& SpawnPoints)
{
	UCouchCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	for	(ACouchPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ACouchCharacter> CouchCharacterClass = GetCouchCharacterClassFromInputType(InputType);
		if (!CouchCharacterClass) continue;

		ACouchCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ACouchCharacter>(
			CouchCharacterClass,
			SpawnPoint->GetTransform()
		);

		if (!NewCharacter) continue;
		NewCharacter->InputData = InputData;
		NewCharacter->InputMappingContext = InputMappingContext;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrient(FVector2D(SpawnPoint->GetStartOrientX(),0));
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInGame.Add(NewCharacter);
	}
	
}

TSubclassOf<ACouchCharacter> ACouchGameMode::GetCouchCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	switch (InputType)
	{
		case EAutoReceiveInput::Player0 :
			return ArenaSettings->CouchCharacterClassPO;
		
		case EAutoReceiveInput::Player1 :
			return ArenaSettings->CouchCharacterClassP1;
		
		case EAutoReceiveInput::Player2 :
			return ArenaSettings->CouchCharacterClassP2;
		
		case EAutoReceiveInput::Player3 :
			return ArenaSettings->CouchCharacterClassP3;

		default:
			return nullptr;
	}
}
