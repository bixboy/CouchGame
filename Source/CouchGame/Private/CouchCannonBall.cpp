// Fill out your copyright notice in the Description page of Project Settings.

#include "CouchCannonBall.h"

#include "CouchStaticCanonBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ACouchCannonBall::ACouchCannonBall()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = Base;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	
	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(RootComponent);
	Down = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	Down->SetupAttachment(RootComponent);
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

void ACouchCannonBall::InitCanonBall(TObjectPtr<ACouchStaticCanonBall> PickableCannonBall)
{
	SetActorTransform(PickableCannonBall->GetActorTransform());
	Base->SetStaticMesh(PickableCannonBall->Base->GetStaticMesh());
	
	
	Top->SetStaticMesh( PickableCannonBall->Top->GetStaticMesh());
	Top->SetRelativeTransform(PickableCannonBall->Top->GetRelativeTransform());

	
	Down->SetStaticMesh( PickableCannonBall->Down->GetStaticMesh());
	Down->SetRelativeTransform(PickableCannonBall->Down->GetRelativeTransform());

}

void ACouchCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;
	FVector NewLocation = Location + (Velocity * TimeElapsed) + (FVector(0, 0, Gravity) * TimeElapsed * TimeElapsed * 0.5f);
	SetActorLocation(NewLocation);

}

