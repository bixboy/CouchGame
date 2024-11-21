// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect/CouchProjectileEffect.h"
#include "CouchProjectileEffectFog.generated.h"

class AFog;
class AStorm;

UCLASS()
class COUCHGAME_API ACouchProjectileEffectFog : public ACouchProjectileEffect
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchProjectileEffectFog();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void ExecuteEffect() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFog> Fog;

	UPROPERTY()
	TObjectPtr<AFog> FogPtr;
	
	UPROPERTY(EditAnywhere)
	float DelayToExecute = 5.0;
	float Timer;
};
