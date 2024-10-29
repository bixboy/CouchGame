// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/CouchBoat.h"

#include "Boat/BoatFloor.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ACouchBoat::ACouchBoat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoatMesh"));
	RootComponent = BoatMesh;
	
	BoatLife = BoatStartLife;
}

// Called when the game starts or when spawned
void ACouchBoat::BeginPlay()
{
	Super::BeginPlay();
	BoatLife = BoatStartLife;
	if (BoatFloor)
	{
		BoatFloor->Init(this);
	}
	else // Rajouter QQ chose s'il le floor du bâteau n'a pas été assigné
	{
		
	}
	
}
void ACouchBoat::BoatDamage(float DamageAmount)
{
	DamageAmount = FMath::Clamp(FMath::Abs(DamageAmount), BoatMinAndMaxDamagePerSecond.X, BoatMinAndMaxDamagePerSecond.Y);
	BoatLife =  FMath::Clamp(BoatLife - DamageAmount, 0, BoatStartLife);
	if (BoatLife == 0)
	{
		SinkBoatAndGameOver();
	}
}

void ACouchBoat::BoatRepair(float HealAmount)
{
	HealAmount = FMath::Abs(HealAmount);
	BoatLife = FMath::Clamp(BoatLife + HealAmount, 0, BoatStartLife);
}

void ACouchBoat::BoatRepair()
{
	BoatRepair(HealAmountPerHitRepaired);
}

void ACouchBoat::SinkBoatAndGameOver()
{
	switch (Team)
	{
		case EBoatTeam::Team1:
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Team 2 Win");
			break;
		}
		case EBoatTeam::Team2:
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Team 1 Win");
			break;
		}
		default:
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "No Team Win");
			break;
		
	}
}

float ACouchBoat::GetBoatLife() const
{
	return BoatLife;
}





