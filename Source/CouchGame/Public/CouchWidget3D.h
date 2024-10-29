// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchWidget3D.generated.h"

UCLASS()
class COUCHGAME_API ACouchWidget3D : public AActor
{
	GENERATED_BODY()

public:
	ACouchWidget3D();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
