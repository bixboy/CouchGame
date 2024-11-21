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

};
