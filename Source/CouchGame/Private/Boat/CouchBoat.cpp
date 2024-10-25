// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/CouchBoat.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACouchBoat::ACouchBoat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
	RootComponent = BoatMesh;
	
	BoatLife = BoatStartLife;
}

// Called when the game starts or when spawned
void ACouchBoat::BeginPlay()
{
	Super::BeginPlay();
	BoatLife = BoatStartLife;
	
}
void ACouchBoat::BoatDamage(float DamageAmount)
{
	DamageAmount = FMath::Abs(DamageAmount);
	BoatLife =  FMath::Clamp(BoatLife - DamageAmount, 0, BoatStartLife);
}

void ACouchBoat::BoatRepair(float HealAmount)
{
	HealAmount = FMath::Abs(HealAmount);
	BoatLife = FMath::Clamp(BoatLife + HealAmount, 0, BoatStartLife);
}

float ACouchBoat::GetBoatLife() const
{
	return BoatLife;
}




