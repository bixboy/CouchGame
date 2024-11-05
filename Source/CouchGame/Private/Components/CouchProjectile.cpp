// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CouchProjectile.h"

UCouchProjectile::UCouchProjectile()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCouchProjectile::Initialize(const FVector& LaunchVelocity)
{
	Velocity = LaunchVelocity;
	Location = GetOwner()->GetActorLocation();
	TimeElapsed = 0.0f;
	CanMove = true;
}

void UCouchProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanMove)
	{
		TimeElapsed += DeltaTime;
		FVector NewLocation = Location + (Velocity * TimeElapsed) + (FVector(0, 0, Gravity) * TimeElapsed * TimeElapsed * 0.5f);
		GetOwner()->SetActorLocation(NewLocation);
	}
}

