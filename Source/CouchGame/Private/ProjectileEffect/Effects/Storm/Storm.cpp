// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Storm/Storm.h"
#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
AStorm::AStorm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(RootComponent);
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->bAutoActivate = false; // On l'active manuellement
	RadialForceComponent->bIgnoreOwningActor = true;
}

void AStorm::Init(float StormStrength, float ForceRadius) const
{
	RadialForceComponent->Radius = ForceRadius;
	RadialForceComponent->ImpulseStrength = StormStrength;
	RadialForceComponent->SetWorldLocation(GetActorLocation());
}


