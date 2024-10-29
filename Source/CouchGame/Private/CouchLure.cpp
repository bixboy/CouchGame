// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchLure.h"

#include "Components/CouchProjectile.h"
#include "Interfaces/CouchInteractable.h"

ACouchLure::ACouchLure()
{
	PrimaryActorTick.bCanEverTick = true;
	CouchProjectile = CreateDefaultSubobject<UCouchProjectile>(TEXT("ProjectileComponent"));
	
	SphereComponent =  CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	LureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LureMesh"));
	LureMesh->SetupAttachment(RootComponent);
	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(RootComponent);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACouchLure::OnLureBeginOverlap);
}

void ACouchLure::BeginPlay()
{
	Super::BeginPlay();
}


void ACouchLure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACouchLure::OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UCouchInteractable>())
	{
		AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

