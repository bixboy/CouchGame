// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchStaticCanonBall.h"
#include "Interactables/CouchPickableCannonBall.h"


// Sets default values
ACouchStaticCanonBall::ACouchStaticCanonBall()
{
	PrimaryActorTick.bCanEverTick = false;

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	RootComponent = Base;

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(RootComponent);

	Down = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	Down->SetupAttachment(RootComponent);
}

void ACouchStaticCanonBall::InitCanonBall(TObjectPtr<ACouchPickableCannonBall> RefPickableCannonBall)
{
	SetActorTransform(RefPickableCannonBall->GetActorTransform());
	Base->SetStaticMesh(RefPickableCannonBall->Mesh->GetStaticMesh());
	Base->SetRelativeTransform(RefPickableCannonBall->Mesh->GetRelativeTransform());

	
	Top->SetStaticMesh( RefPickableCannonBall->Top->GetStaticMesh());
	Top->SetRelativeTransform(RefPickableCannonBall->Top->GetRelativeTransform());

	
	Down->SetStaticMesh( RefPickableCannonBall->Down->GetStaticMesh());
	Down->SetRelativeTransform(RefPickableCannonBall->Down->GetRelativeTransform());

	ProjectileEffects = RefPickableCannonBall->ProjectileEffects;
	PickableCannonBall = RefPickableCannonBall;
}




