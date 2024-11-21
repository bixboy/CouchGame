// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Fog/CouchProjectileEffectFog.h"
#include "CouchCannonBall.h"
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
		FogPtr->UpdateFog(MaxOpacity * (1.0f - FMath::Clamp(Timer / DelayToExecute, 0.0f, 1.0f)), MaxOpacity);
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
	CouchCannonBall->Destroy();
}




