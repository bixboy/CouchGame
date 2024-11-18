// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/CouchProjectileEffect.h"

#include "CouchCannonBall.h"


// Sets default values
ACouchProjectileEffect::ACouchProjectileEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACouchProjectileEffect::ExecuteEffect()
{
	SetActorTransform(CouchCannonBall->GetActorTransform());
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Projectile Effect");
	HasExecutedEffect = true;
}

float ACouchProjectileEffect::GetDelay() const
{
	return Delay;
}

void ACouchProjectileEffect::Init(ACouchCannonBall* InBall)
{
	CouchCannonBall = InBall;
}

void ACouchProjectileEffect::Init(ACouchPickableCannonBall* InBall)
{
	PickableCannonBall = InBall;
}

void ACouchProjectileEffect::Init(AActor* InActor)
{
	ActorToInterractWith = InActor;
}

void ACouchProjectileEffect::Init(FHitResult InHitResult)
{
	HitResult = InHitResult;
}

void ACouchProjectileEffect::BeginPlay()
{
	Super::BeginPlay();
}

void ACouchProjectileEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ExecuteTime == ECouchProjectileExecuteTime::OnDelay && !HasExecutedEffect)
	{
		Timer += DeltaTime;
		if (Timer >= Delay)
		{
			ExecuteEffect();
		}
	}
}
