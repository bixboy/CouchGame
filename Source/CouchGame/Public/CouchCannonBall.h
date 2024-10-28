// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchCannonBall.generated.h"

UCLASS()
class COUCHGAME_API ACouchCannonBall : public AActor
{
	GENERATED_BODY()

public:
	ACouchCannonBall();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void Initialize(const FVector& LaunchVelocity);

private:
	FVector Velocity;      // Vélocité du projectile
	FVector Location;      // Position actuelle du projectile
	float TimeElapsed;     // Temps écoulé depuis le lancement
	const float Gravity = -980.0f;
};
