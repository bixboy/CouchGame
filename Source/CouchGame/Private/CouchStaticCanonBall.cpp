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

	CopyMeshData(Base, RefPickableCannonBall->Mesh);
	CopyMeshData(Top, RefPickableCannonBall->Top);
	CopyMeshData(Down, RefPickableCannonBall->Down);
	
	ProjectileEffects = RefPickableCannonBall->ProjectileEffects;
	PickableCannonBall = RefPickableCannonBall;
}

void ACouchStaticCanonBall::CopyMeshData(UStaticMeshComponent* Target, UStaticMeshComponent* Source)
{
	if (!Target || !Source) return;

	// Copier le StaticMesh
	Target->SetStaticMesh(Source->GetStaticMesh());

	// Copier la transformation relative
	Target->SetRelativeTransform(Source->GetRelativeTransform());

	// Copier tous les matériaux d'un seul coup
	const TArray<UMaterialInterface*>& Materials = Source->GetMaterials();
	for (int32 i = 0; i < Materials.Num(); i++)
	{
		Target->SetMaterial(i, Materials[i]);
	}
}




