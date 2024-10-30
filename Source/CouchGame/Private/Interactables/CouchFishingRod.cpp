// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/CouchFishingRod.h"


// Sets default values
ACouchFishingRod::ACouchFishingRod()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACouchFishingRod::BeginPlay()
{
	Super::BeginPlay();
	
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
	Super::Tick(DeltaTime);
}

