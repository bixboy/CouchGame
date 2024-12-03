// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CouchGameModeMenu.generated.h"

class ACouchPlayerStart;
class UInputMappingContext;
class UCouchCharacterInputData;
class ACouchCharacter;
/**
 * 
 */
UCLASS()
class COUCHGAME_API ACouchGameModeMenu : public AGameModeBase
{
	GENERATED_BODY()
	public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TArray<ACouchCharacter*> CharactersInGame;

private:
	UCouchCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();
	
	void FindPlayerStartActorsInArena(TArray<ACouchPlayerStart*>& ResultsActors);

	void SpawnCharacter(const TArray<ACouchPlayerStart*>& SpawnPoints);

	void CreateAndInitPlayers() const;
	
};
