// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/CouchGameMode.h"

#include "Arena/ArenaSettings.h"
#include "CouchGame/Public/Arena//CouchPlayerStart.h"
#include "CouchGame/Public/Characters/CouchCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CouchCharacterInputData.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSubsystem.h"
#include "Arena/CouchGameManagerSubSystem.h"
#include "Characters/CouchCharacterSettings.h"


void ACouchGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreateAndInitPlayers();
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
	if (SpawnPoints.Num() == 0) return;
	UCouchCharacterInputData* InputData = LoadInputDataFromConfig();
	// UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	for	(int i = 0; i < GameManager->NbPlayers; i++)
	{
		ACouchPlayerStart * SpawnPoint = Cast<ACouchPlayerStart>(SpawnPoints[i]);
		if (!SpawnPoint) return ;
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ACouchCharacter> CouchCharacterClass = GetCouchCharacterClassFromInputType(InputType);
		if (!CouchCharacterClass) continue;

		ACouchCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ACouchCharacter>(
			CouchCharacterClass,
			SpawnPoint->GetTransform()
		);

		if (!NewCharacter) continue;
		NewCharacter->InputData = InputData;
		// NewCharacter->InputMappingContext = InputMappingContext;
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrient(FVector2D(SpawnPoint->GetStartOrientX(),0));
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInGame.Add(NewCharacter);
	}
	
}

TSubclassOf<ACouchCharacter> ACouchGameMode::GetCouchCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	const UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	if (!GameManager) return nullptr;
	switch (InputType)
	{
		case EAutoReceiveInput::Player0 :
			return GameManager->CouchCharacterClassPO;
		
		case EAutoReceiveInput::Player1 :
			return GameManager->CouchCharacterClassP1;
		
		case EAutoReceiveInput::Player2 :
			return GameManager->CouchCharacterClassP2;
		
		case EAutoReceiveInput::Player3 :
			return GameManager->CouchCharacterClassP3;

		default:
			return nullptr;
	}
}

void ACouchGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance
	->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (!LocalMultiplayerSubsystem) return;
	const UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	if (!GameManager) return;
	LocalMultiplayerSubsystem->CreateAndInitPlayers(GameManager->MappingType);
}
