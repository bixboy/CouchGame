// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Fog/CouchProjectileEffectFog.h"
#include "CouchCannonBall.h"
#include "EngineUtils.h"
#include "Boat/BoatFloor.h"
#include "ProjectileEffect/Effects/Fog/Fog.h"


// Sets default values
ACouchProjectileEffectFog::ACouchProjectileEffectFog()
{
	PrimaryActorTick.bCanEverTick = true;
	Timer = 0;
}

// Called every frame
void ACouchProjectileEffectFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasExecutedEffect && FogPtr)
	{
		Timer += DeltaTime;
		//FogPtr->UpdateFog(MaxOpacity * (1.0f - FMath::Clamp(Timer / DelayToExecute, 0.0f, 1.0f)), MaxOpacity);
		if (Timer >= DelayToExecute)
		{
			FogPtr->Destroy();
			Destroy();
		}
	}
}

void ACouchProjectileEffectFog::ExecuteEffect()
{
	Super::ExecuteEffect();
	FTransform Transform = FTransform(FRotator::ZeroRotator, CouchCannonBall->GetActorLocation(), FVector(1.0f));
	FogPtr = GetWorld()->SpawnActor<AFog>(Fog, Transform);
	ABoatFloor* ClosestBoatFloor = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();
	for (TActorIterator<ABoatFloor> It(GetWorld()); It; ++It)
	{
		ABoatFloor* BoatFloor = *It;
		if (BoatFloor)
		{
			float Distance = FVector::Dist(BoatFloor->GetActorLocation(), FogPtr->GetActorLocation());
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestBoatFloor = BoatFloor;
			}
		}
	}
	if (ClosestBoatFloor)
	{
		FogPtr->AttachToActor(ClosestBoatFloor, FAttachmentTransformRules::KeepWorldTransform);
	}
	if (CouchCannonBall) CouchCannonBall->Destroy();
}




