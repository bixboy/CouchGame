// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/CouchBoat.h"

#include "Arena/CouchGameManagerSubSystem.h"
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
	InitialRotation = GetActorRotation();
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

void ACouchBoat::UpdateBoatRotation(float Value)
{

	// Calcul de la nouvelle rotation
	FRotator NewRotation = InitialRotation;
	NewRotation.Pitch += (Value * ForceStrength) * 0.3f;
	NewRotation.Roll += Value * ForceStrength * 0.3f;
	
	SetActorRotation(NewRotation);
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
	if (IsDestroyed) return;
	
	UCouchGameManagerSubSystem* GameManager = GetGameInstance()->GetSubsystem<UCouchGameManagerSubSystem>();
	switch (Team)
	{
		case EBoatTeam::Team1:
		{
			// Team 2 Win	
			GameManager->CheckRoundWinCondition(2);
			break;
		}
		case EBoatTeam::Team2:
		{
			// Team 1 Win		
			GameManager->CheckRoundWinCondition(1);
			break;
		}
		default:
			// no Team Win	
			GameManager->CheckRoundWinCondition(1);
			break;
		
	}
	IsDestroyed = true;
}

float ACouchBoat::GetBoatLife() const
{
	return BoatLife;
}





