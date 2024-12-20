// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CouchGameMode.generated.h"

class UInputMappingContext;
class UCouchCharacterInputData;
class ACouchPlayerStart;
class ACouchCharacter;
/**
 * 
 */
UCLASS()
class COUCHGAME_API ACouchGameMode : public AGameModeBase
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

	TSubclassOf<ACouchCharacter> GetCouchCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;

	void CreateAndInitPlayers() const;
	
};
