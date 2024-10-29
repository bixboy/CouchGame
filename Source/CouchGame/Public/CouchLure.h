// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CouchProjectile.h"
#include "GameFramework/Actor.h"
#include "CouchLure.generated.h"

UCLASS()
class COUCHGAME_API ACouchLure : public AActor
{
	GENERATED_BODY()

public:
	ACouchLure();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCouchProjectile* CouchProjectile;

protected:
	virtual void BeginPlay() override;

private:

};
