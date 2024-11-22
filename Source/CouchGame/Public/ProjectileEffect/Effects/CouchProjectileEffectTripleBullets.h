// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect/CouchProjectileEffect.h"
#include "CouchProjectileEffectTripleBullets.generated.h"

UCLASS()
class COUCHGAME_API ACouchProjectileEffectTripleBullets : public ACouchProjectileEffect
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchProjectileEffectTripleBullets();
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 1))
	float Degree = 0.25f;
	
};
