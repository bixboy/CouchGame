// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/BoatFloor.h"

#include "Boat/CouchBoat.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABoatFloor::ABoatFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor StaticMesh"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorCollision"));
	RootComponent = Collision;
	StaticMesh->SetupAttachment(Collision);
}

void ABoatFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ABoat && Hits.Num() > 0)
	{
		Timer += DeltaTime;
		if (Timer >= DamageFrequency)
		{
			// ABoat->BoatDamage(Hits.Num());
			for(int i = 0; i < Hits.Num(); i++)
			{
				FVector Scale = Hits[i]->HitMesh->GetRelativeScale3D();
				float AverageScale = (Scale.X + Scale.Y + Scale.Z) / 3.0f;
				ABoat->BoatDamage(AverageScale);
			}
			Timer = 0;
		}
	}
}

void ABoatFloor::Init(ACouchBoat* Boat)
{
	ABoat = Boat;
}

ACouchPlank* ABoatFloor::Hit_Implementation(FHitResult HitResult, float RepairingTime, float Scale)
{
	ICouchDamageable::Hit_Implementation(HitResult, RepairingTime, Scale);
	FString HitLocationString = FString::Printf(TEXT("Boat Hit at Location: X=%.2f, Y=%.2f, Z=%.2f"), 
											HitResult.Location.X, 
											HitResult.Location.Y, 
											HitResult.Location.Z);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitLocationString);
	
	if (FloorHit)
	{
		FTransform SpawnTransform(GetActorRotation(), HitResult.ImpactPoint);
		SpawnTransform.SetLocation(SpawnTransform.GetLocation() + PlankLocationOffset);
		if (SpawnTransform.GetLocation() != FVector::ZeroVector)
		{
			ACouchPlank* NewHit =  GetWorld()->SpawnActor<ACouchPlank>(FloorHit, SpawnTransform);
			NewHit->Init(this, RepairingTime, Scale);
			Hits.Add(NewHit);
			return NewHit;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("FloorHit class is null."));
	}
	return nullptr;
}

void ABoatFloor::RemoveHitFromArray(ACouchPlank* Plank)
{
	Hits.Remove(Plank);
	ABoat->BoatRepair();
}


void ABoatFloor::BeginPlay()
{
	Super::BeginPlay();
}

