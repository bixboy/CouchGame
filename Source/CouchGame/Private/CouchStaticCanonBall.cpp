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

void ACouchStaticCanonBall::InitCanonBall(TObjectPtr<ACouchPickableCannonBall> PickableCannonBall)
{
	SetActorTransform(PickableCannonBall->GetActorTransform());
	Base->SetStaticMesh(PickableCannonBall->Base->GetStaticMesh());
	Base->SetRelativeTransform(PickableCannonBall->Base->GetRelativeTransform());

	
	Top->SetStaticMesh( PickableCannonBall->Top->GetStaticMesh());
	Top->SetRelativeTransform(PickableCannonBall->Top->GetRelativeTransform());

	
	Down->SetStaticMesh( PickableCannonBall->Down->GetStaticMesh());
	Down->SetRelativeTransform(PickableCannonBall->Down->GetRelativeTransform());

}




