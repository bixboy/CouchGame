// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchFishingRod.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CouchLure.h"
#include "Kismet/GameplayStatics.h"

ACouchFishingRod::ACouchFishingRod()
{
	PrimaryActorTick.bCanEverTick = false;
	ChargePower = CreateDefaultSubobject<UCouchChargePower>(TEXT("ChargePower"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = SkeletalMesh;
}

void ACouchFishingRod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACouchFishingRod::StartChargeActor_Implementation()
{
	ICouchInteractable::StartChargeActor_Implementation();
	ChargePower->StartCharging(SkeletalMesh);
}

void ACouchFishingRod::StopChargeActor_Implementation()
{
	ICouchInteractable::StopChargeActor_Implementation();
	ChargePower->StopCharging();
}

void ACouchFishingRod::SpawnLure()
{
	CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Cable->SetupAttachment(RootComponent);
	Cable->SetRelativeScale3D(FVector(CableScale, CableScale, CableScale));
	Cable->SetMaterial(0, CableMaterial);

	Lure = GetWorld()->SpawnActor<ACouchLure>();
	Cable->SetAttachEndTo(Lure, FName(TEXT("None")), FName(TEXT("Attach")));
	Cable->SetVisibility(true, true);

	PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ChargePower"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	PhysicsConstraint->SetConstrainedComponents(SkeletalMesh, FName(TEXT("None")), Lure->GetComponentByClass<USkeletalMeshComponent>(), FName(TEXT("None")));

	FVector StartLocation = SkeletalMesh->GetSocketLocation(FName("barrel"));
	FVector SuggestedVelocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		SuggestedVelocity,
		StartLocation,
		ChargePower->TargetLocation,
		0,
		0.5
	);
	Lure->CouchProjectile->Initialize(SuggestedVelocity);
}

