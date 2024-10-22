// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame/Public/CouchGameMode.h"
#include "CouchGame/Public/CouchCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


void ACouchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<APlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacter(PlayerStartsPoints);
}

void ACouchGameMode::FindPlayerStartActorsInArena(TArray<APlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		APlayerStart* PlayerStartActor = Cast<APlayerStart>(FoundActors[i]);
		if (!PlayerStartActor) continue;
		
		ResultsActors.Add(PlayerStartActor);
	}
}

void ACouchGameMode::SpawnCharacter(const TArray<APlayerStart*>& SpawnPoints)
{
	for	(APlayerStart* SpawnPoint : SpawnPoints)
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
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		CharactersInGame.Add(NewCharacter);
	}
	
}

TSubclassOf<ACouchCharacter> ACouchGameMode::GetCouchCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	switch (InputType)
	{
		case EAutoReceiveInput::Player0 :
			return CouchCharacterP0;
		
		case EAutoReceiveInput::Player1 :
			return CouchCharacterP1;
		
		case EAutoReceiveInput::Player2 :
			return CouchCharacterP2;
		
		case EAutoReceiveInput::Player3 :
			return CouchCharacterP3;

		default:
			return nullptr;
	}
}
