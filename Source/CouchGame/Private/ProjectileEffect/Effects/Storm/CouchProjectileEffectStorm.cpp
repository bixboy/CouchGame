// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Storm/CouchProjectileEffectStorm.h"
#include "CouchCannonBall.h"
#include "ProjectileEffect/Effects/Storm/Storm.h"


// Sets default values
ACouchProjectileEffectStorm::ACouchProjectileEffectStorm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Timer = 0;
	// Initialisation du Radial Force Component
}

// Called every frame
void ACouchProjectileEffectStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasExecutedEffect && StormPtr)
	{
		Timer += DeltaTime;
		if (Timer >= DelayToExecute)
		{
			StormPtr->Destroy();
			Destroy();
		}
	}
}

void ACouchProjectileEffectStorm::ExecuteEffect()
{
	Super::ExecuteEffect();
	FTransform Transform = FTransform(FRotator::ZeroRotator, CouchCannonBall->GetActorLocation(), FVector(1.0f));
	StormPtr = GetWorld()->SpawnActor<AStorm>(Storm, Transform);
	if (StormPtr)
	{
		StormPtr->Init(StormStrength, ForceRadius);
		StormPtr->RadialForceComponent->Activate(true);
	}
	CouchCannonBall->Destroy();
}




