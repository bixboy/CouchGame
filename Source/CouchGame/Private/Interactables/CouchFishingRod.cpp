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
	
	FTransform SpawnTransform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
	LureRef = GetWorld()->SpawnActor<ACouchLure>(Lure, SpawnTransform);
	LureRef->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	InitializeCableAndConstraint();
	
	LureRef->CouchProjectile->Initialize(SuggestedVelocity);
}

void ACouchFishingRod::InitializeCableAndConstraint()
{
    // Ajoute le composant Cable
	if ((Cable = NewObject<UCableComponent>(this, TEXT("Cable"))))
	{
		Cable->SetupAttachment(RootComponent);
		Cable->RegisterComponent();
		Cable->SetRelativeScale3D(FVector(CableScale, CableScale, CableScale));

		// Définit le matériau du câble
		if (CableMaterial)
		{
			Cable->SetMaterial(0, CableMaterial);
		}

		Cable->CableLength = 10.f;

		// Attache l'extrémité du câble
		Cable->SetAttachEndToComponent(LureRef->SphereComponent);
		    //Cable->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("barrel"));
		    //Cable->SetAttachEndTo(LureRef, FName(TEXT("TopMesh")), FName(TEXT("barrel")));

		Cable->EndLocation = FVector(0.0f, 0.0f, 2.0f);
		Cable->SetVisibility(true, true);	
	}

    // Ajoute le composant PhysicsConstraint
    if ((PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this, TEXT("PhysicsConstraint"))))
    {
    	PhysicsConstraint->SetupAttachment(RootComponent);
    	PhysicsConstraint->RegisterComponent();

    	// Configure les composants contraints
    	PhysicsConstraint->SetConstrainedComponents(SkeletalMesh, FName(TEXT("None")), LureRef->SphereComponent, FName(TEXT("None")));   
    }
}

void ACouchFishingRod::RewindCable(float JoystickInput)
{
	if (JoystickInput > Threshold)
	{
		float NewCableLength = FMath::Clamp(Cable->CableLength - RewindSpeed * JoystickInput, MinCableLength, MaxCableLength);
		Cable->CableLength = NewCableLength;
	}
}

