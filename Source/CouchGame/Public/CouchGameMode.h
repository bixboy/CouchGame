// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CouchGameMode.generated.h"

class ACouchCharacter;
class APlayerStart;
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
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACouchCharacter> CouchCharacterP0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACouchCharacter> CouchCharacterP1;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACouchCharacter> CouchCharacterP2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACouchCharacter> CouchCharacterP3;

private:
	void FindPlayerStartActorsInArena(TArray<APlayerStart*>& ResultsActors);

	void SpawnCharacter(const TArray<APlayerStart*>& SpawnPoints);

	TSubclassOf<ACouchCharacter> GetCouchCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;
	
};
