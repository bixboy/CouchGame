// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/BoatFloor.h"


// Sets default values
ABoatFloor::ABoatFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoatFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoatFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoatFloor::Hit_Implementation(FHitResult HitResult)
{
	ICouchDamageable::Hit_Implementation(HitResult);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Boat Hit");
}

