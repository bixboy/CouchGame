// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGameMode.h"

#include "Arena/ArenaSettings.h"
#include "CouchGame/Public/CouchPlayerStart.h"
#include "CouchGame/Public/CouchCharacter.h"
#include "Kismet/GameplayStatics.h"


void ACouchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<ACouchPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacter(PlayerStartsPoints);
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
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
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
