// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchFishingRod.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CouchLure.h"
#include "Components/CouchChargePower.h"
#include "Interfaces/CouchInteractable.h"
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
	SpawnLure();
}

void ACouchFishingRod::SpawnLure()
{
	Cable = NewObject<UCableComponent>(this, TEXT("Cable"));
	Cable->SetupAttachment(RootComponent);
	Cable->SetRelativeScale3D(FVector(CableScale, CableScale, CableScale));
	Cable->SetMaterial(0, CableMaterial);

	FTransform SpawnTransform = FTransform(FRotator(0, 0, 0), ChargePower->TargetLocation);
	LureRef = GetWorld()->SpawnActor<ACouchLure>(Lure,SpawnTransform);
	Cable->SetAttachEndTo(LureRef, FName(TEXT("None")), FName(TEXT("barrel")));
	Cable->SetVisibility(true, true);

	PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this, TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	PhysicsConstraint->SetConstrainedComponents(SkeletalMesh, FName(TEXT("Tip")), LureRef->TopMesh, FName(TEXT("None")));

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
	LureRef->CouchProjectile->Initialize(SuggestedVelocity);
}

void ACouchFishingRod::RewindCable(float JoystickInput)
{
	if (JoystickInput > Threshold)
	{
		float NewCableLength = FMath::Clamp(Cable->CableLength - RewindSpeed * JoystickInput, MinCableLength, MaxCableLength);
		Cable->CableLength = NewCableLength;
	}
}

