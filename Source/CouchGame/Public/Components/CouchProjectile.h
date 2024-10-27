// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CouchProjectile.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME_API UCouchProjectile : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UCouchProjectile();
	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void Initialize(const FVector& LaunchVelocity);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector Velocity;      // Vélocité du projectile
	FVector Location;      // Position actuelle du projectile
	float TimeElapsed;     // Temps écoulé depuis le lancement
	const float Gravity = -980.0f;
};
