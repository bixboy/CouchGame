// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ArenaSettings.generated.h"

class ACouchCharacter;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Couch Arena Settings"))
class COUCHGAME_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Characters")
	TSubclassOf<ACouchCharacter> CouchCharacterClassPO;

	UPROPERTY(Config, EditAnywhere, Category = "Characters")
	TSubclassOf<ACouchCharacter> CouchCharacterClassP1;

	UPROPERTY(Config, EditAnywhere, Category = "Characters")
	TSubclassOf<ACouchCharacter> CouchCharacterClassP2;

	UPROPERTY(Config, EditAnywhere, Category = "Characters")
	TSubclassOf<ACouchCharacter> CouchCharacterClassP3;
};
