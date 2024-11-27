// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect/CouchProjectileEffect.h"
#include "CouchProjectileEffectStorm.generated.h"

class ACouchPlank;
class AStorm;

UCLASS()
class COUCHGAME_API ACouchProjectileEffectStorm : public ACouchProjectileEffect
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchProjectileEffectStorm();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void ExecuteEffect() override;
	
	UPROPERTY()
	TObjectPtr<ACouchPlank> HitPlankPtr;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStorm> Storm;

	UPROPERTY()
	TObjectPtr<AStorm> StormPtr;

	
	UPROPERTY(EditAnywhere)
	float DelayToExecute = 5.0;
	float Timer;
	
	UPROPERTY(EditAnywhere, Category = "Storm")
	float StormStrength = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Storm")
	float ForceRadius = 500.0f;
	

};
