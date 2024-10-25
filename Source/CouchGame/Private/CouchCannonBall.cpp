// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchCannonBall.h"


ACouchCannonBall::ACouchCannonBall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACouchCannonBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACouchCannonBall::Initialize(const FVector& LaunchVelocity)
{
	Velocity = LaunchVelocity;  // Définir la vélocité initiale
	Location = GetActorLocation(); // Obtenir la position initiale
	TimeElapsed = 0.0f; // Réinitialiser le temps écoulé
}

void ACouchCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;
	FVector NewLocation = Location + (Velocity * TimeElapsed) + (FVector(0, 0, Gravity) * TimeElapsed * TimeElapsed * 0.5f);
	SetActorLocation(NewLocation);

}
