// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect/CouchProjectileEffect.h"
#include "CouchProjectileEffectMultiplicator.generated.h"

UCLASS()
class COUCHGAME_API ACouchProjectileEffectMultiplicator : public ACouchProjectileEffect
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchProjectileEffectMultiplicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ExecuteEffect() override;

	UPROPERTY(EditAnywhere)
	float TimeToRepair = 2.0f;
	
	UPROPERTY(EditAnywhere)
	float Scale = 2.0f;


};
