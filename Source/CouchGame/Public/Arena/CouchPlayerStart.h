// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchPlayerStartOrient.h"
#include "Boat/CouchBoat.h"
#include "GameFramework/PlayerStart.h"
#include "CouchPlayerStart.generated.h"


UCLASS()
class COUCHGAME_API ACouchPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UFUNCTION()
	float GetStartOrientX() const;
	
	UPROPERTY(EditAnywhere)
	ECouchPlayerStartOrient StartOrient = ECouchPlayerStartOrient::Right;

	UPROPERTY(EditAnywhere)
	EBoatTeam Team = EBoatTeam::Team1;
};


