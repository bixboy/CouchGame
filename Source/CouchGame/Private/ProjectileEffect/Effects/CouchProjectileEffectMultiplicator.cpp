// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/CouchProjectileEffectMultiplicator.h"

#include "CouchCannonBall.h"
#include "Interfaces/CouchDamageable.h"


// Sets default values
ACouchProjectileEffectMultiplicator::ACouchProjectileEffectMultiplicator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACouchProjectileEffectMultiplicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACouchProjectileEffectMultiplicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACouchProjectileEffectMultiplicator::ExecuteEffect()
{
	Super::ExecuteEffect();
	if (ActorToInterractWith && HitResult.IsValidBlockingHit())
	{
		ICouchDamageable::Execute_Hit(ActorToInterractWith, HitResult, TimeToRepair, Scale);
	}
	CouchCannonBall->Destroy();
	Destroy();
}

