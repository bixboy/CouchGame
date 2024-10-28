// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/BoatFloor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABoatFloor::ABoatFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor StaticMesh"));
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorCollision"));
	RootComponent = Collision;
	StaticMesh->SetupAttachment(Collision);
}

void ABoatFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoatFloor::Hit_Implementation(FHitResult HitResult)
{
	ICouchDamageable::Hit_Implementation(HitResult);
	
	FString HitLocationString = FString::Printf(TEXT("Boat Hit at Location: X=%.2f, Y=%.2f, Z=%.2f"), 
												HitResult.Location.X, 
												HitResult.Location.Y, 
												HitResult.Location.Z);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HitLocationString);
	
	if (FloorHit)
	{
		FTransform SpawnTransform(GetActorRotation(), HitResult.ImpactPoint);
		if (SpawnTransform.GetLocation() != FVector::ZeroVector)
		{
			ACouchPlank* NewHit =  GetWorld()->SpawnActor<ACouchPlank>(FloorHit, SpawnTransform);
			Hits.Add(NewHit);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("FloorHit class is null."));
	}
}


void ABoatFloor::BeginPlay()
{
	Super::BeginPlay();
}

