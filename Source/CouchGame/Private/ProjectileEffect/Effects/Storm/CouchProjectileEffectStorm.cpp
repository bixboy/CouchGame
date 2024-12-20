// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Storm/CouchProjectileEffectStorm.h"
#include "CouchCannonBall.h"
#include "EngineUtils.h"
#include "Boat/BoatFloor.h"
#include "ProjectileEffect/Effects/Storm/Storm.h"


class ABoatFloor;
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
		ABoatFloor* ClosestBoatFloor = nullptr;
		float ClosestDistance = TNumericLimits<float>::Max();
		for (TActorIterator<ABoatFloor> It(GetWorld()); It; ++It)
		{
			ABoatFloor* BoatFloor = *It;
			if (BoatFloor)
			{
				float Distance = FVector::Dist(BoatFloor->GetActorLocation(), StormPtr->GetActorLocation());
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					ClosestBoatFloor = BoatFloor;
				}
			}
		}
		// Si un ABoatFloor est trouvé, attache la tempête
		if (ClosestBoatFloor)
		{
			StormPtr->AttachToActor(ClosestBoatFloor, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	if (CouchCannonBall) CouchCannonBall->Destroy();
}




