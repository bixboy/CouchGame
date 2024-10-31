// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchInteractableWeapons.h"
#include "CouchUmbrella.generated.h"

UCLASS()
class COUCHGAME_API ACouchUmbrella : public ACouchInteractableWeapons
{
	GENERATED_BODY()

public:
	ACouchUmbrella();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* ShieldBox;
};
