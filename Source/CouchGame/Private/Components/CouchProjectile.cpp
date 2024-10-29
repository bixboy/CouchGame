// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CouchProjectile.h"

UCouchProjectile::UCouchProjectile()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCouchProjectile::Initialize(const FVector& LaunchVelocity)
{
	Velocity = LaunchVelocity;  // Définir la vélocité initiale
	Location = GetOwner()->GetActorLocation(); // Obtenir la position initiale
	TimeElapsed = 0.0f; // Réinitialiser le temps écoulé
}

void UCouchProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeElapsed += DeltaTime;
	FVector NewLocation = Location + (Velocity * TimeElapsed) + (FVector(0, 0, Gravity) * TimeElapsed * TimeElapsed * 0.5f);
	SetWorldLocation(NewLocation);
}

