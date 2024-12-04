// Fill out your copyright notice in the Description page of Project Settings.


#include "Arena/CouchGameMode.h"

#include "Arena/ArenaSettings.h"
#include "CouchGame/Public/Arena//CouchPlayerStart.h"
#include "CouchGame/Public/Characters/CouchCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CouchCharacterInputData.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSubsystem.h"
#include "SkeletalMeshTypes.h"
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
	SortPlayerStartsByAutoReceiveInput(ResultsActors);
}

void ACouchGameMode::SortPlayerStartsByAutoReceiveInput(TArray<ACouchPlayerStart*>& PlayerStarts)
{
	// Vérifie que la liste n'est pas vide
	if (PlayerStarts.Num() == 0) return;

	// Tri des PlayerStarts en fonction de leur AutoReceiveInput
	PlayerStarts.Sort([](const ACouchPlayerStart& A, const ACouchPlayerStart& B)
	{
		return static_cast<int32>(A.AutoReceiveInput) < static_cast<int32>(B.AutoReceiveInput);
	});
}

int ACouchGameMode::GetSpawnPointIndex(FCouchPlayerData PlayerData)
{
	if (PlayerData.TeamIndex <= 0 ||  PlayerData.CardIndex < 0) return -1;
	if (PlayerData.TeamIndex == 1)
	{
		if (PlayerData.CardIndex == 0) return 1;
		if (PlayerData.CardIndex == 1) return 0;
		return -1;
	}
	if (PlayerData.TeamIndex == 2)
	{
		if (PlayerData.CardIndex == 0) return 3;
		if (PlayerData.CardIndex == 1) return 2;
		return -1;
	}
	return -1;
}

void ACouchGameMode::SpawnCharacter(const TArray<ACouchPlayerStart*>& SpawnPoints)
{
	if (SpawnPoints.Num() == 0) return;
	UCouchCharacterInputData* InputData = LoadInputDataFromConfig();
	// UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	for	(int i = 0; i < GameManager->Players.Num(); i++)
	{
		if (const int j = GetSpawnPointIndex(GameManager->Players[i]); j != -1)
		{
			auto PlayerData = GameManager->Players[i];
			ACouchPlayerStart* SpawnPoint = Cast<ACouchPlayerStart>(SpawnPoints[j]);
			if (!SpawnPoint) return ;
			
			SpawnPoint->AutoReceiveInput = PlayerData.InputType;
			
			TSubclassOf<ACouchCharacter> CouchCharacterClass = GameManager->CouchCharacterClass;
			if (!CouchCharacterClass) continue;
			ACouchCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ACouchCharacter>(
				CouchCharacterClass,
				SpawnPoint->GetTransform()
			);

			if (!NewCharacter) continue;
			switch (SpawnPoint->Team)
			{
				case EBoatTeam::Team1:
					{
						NewCharacter->SetCurrentTeam(1);
						break;
					}
				case EBoatTeam::Team2:
					{
						NewCharacter->SetCurrentTeam(2);
						break;
					}
				default:
					break;
			}
			NewCharacter->InputData = InputData;
			// NewCharacter->InputMappingContext = InputMappingContext;
			NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
			NewCharacter->SetOrient(FVector2D(SpawnPoint->GetStartOrientX(),0));
			NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
			if (PlayerData.SkeletalMeshAsset) NewCharacter->GetMesh()
			->SetSkeletalMeshAsset(PlayerData.SkeletalMeshAsset);
			CharactersInGame.Add(NewCharacter);
		}
		else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Player Data not Valid");
	}
}

// TSubclassOf<ACouchCharacter> ACouchGameMode::GetCouchCharacterClassFromInputType(
// 	EAutoReceiveInput::Type InputType) const
// {
// 	const UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
// 	if (!GameManager) return nullptr;
// 	switch (InputType)
// 	{
// 		case EAutoReceiveInput::Player0 :
// 			return GameManager->CouchCharacterClassPO;
// 		
// 		case EAutoReceiveInput::Player1 :
// 			return GameManager->CouchCharacterClassP1;
// 		
// 		case EAutoReceiveInput::Player2 :
// 			return GameManager->CouchCharacterClassP2;
// 		
// 		case EAutoReceiveInput::Player3 :
// 			return GameManager->CouchCharacterClassP3;
//
// 		default:
// 			return nullptr;
// 	}
// }

void ACouchGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (!GameInstance) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance
	->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (!LocalMultiplayerSubsystem) return;
	const UCouchGameManagerSubSystem* GameManager = GameInstance->GetSubsystem<UCouchGameManagerSubSystem>();
	if (!GameManager) return;
	LocalMultiplayerSubsystem->CreateAndInitPlayers(GameManager->MappingType);
}
