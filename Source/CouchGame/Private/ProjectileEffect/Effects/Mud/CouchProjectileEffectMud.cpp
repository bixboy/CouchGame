// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Mud/CouchProjectileEffectMud.h"

#include "CouchCannonBall.h"
#include "ProjectileEffect/Effects/Mud/Mud.h"


// Sets default values
ACouchProjectileEffectMud::ACouchProjectileEffectMud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called every frame
void ACouchProjectileEffectMud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasExecutedEffect && MudPtr)
	{
		Timer+= DeltaTime;
		if (Timer >= DelayToExecute)
		{
			MudPtr->Destroy();
			Destroy();
		}
	}
}

void ACouchProjectileEffectMud::ExecuteEffect()
{
	Super::ExecuteEffect();
	FTransform Transform = FTransform(FRotator::ZeroRotator, CouchCannonBall->GetActorLocation(), FVector(1.0f));
	MudPtr = GetWorld()->SpawnActor<AMud>(Mud, Transform);
	if (MudPtr)
	{
		MudPtr->Init(PlayerSlowingSpeed, ItemsSlowingSpeed);
	}
	if (CouchCannonBall) CouchCannonBall->Destroy();
}

