// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CouchCharacterSettings.generated.h"

class UCouchCharacterInputData;
class UInputMappingContext;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Couch Character Settings"))
class COUCHGAME_API UCouchCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Inputs")
	TSoftObjectPtr<UCouchCharacterInputData> InputData;

	UPROPERTY(Config, EditAnywhere, Category="Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;
};
