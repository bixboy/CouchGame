// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchSplineLinePath.generated.h"

UCLASS()
class COUCHGAME_API ACouchSplineLinePath : public AActor
{
	GENERATED_BODY()

public:
	ACouchSplineLinePath();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
