#include "CouchLure.h"

#include "Components/CouchProjectile.h"
#include "Interactables/CouchInteractableMaster.h"
#include "Interactables/CouchPickableMaster.h"
#include "Interfaces/CouchPickable.h"

ACouchLure::ACouchLure()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupLure();
}

#pragma region Setup Lure

void ACouchLure::SetupLure()
{
	CouchProjectile = CreateDefaultSubobject<UCouchProjectile>(TEXT("ProjectileComponent"));
	WidgetSpawner = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("WidgetSpawner"));
  
	SphereComponent =  CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	LureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LureMesh"));
	LureMesh->SetupAttachment(RootComponent);
	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(RootComponent);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACouchLure::OnLureBeginOverlap);
}

void ACouchLure::Initialize(const FVector& LaunchVelocity, ACouchFishingRod* FishingRod)
{
	TArray<TObjectPtr<AActor>> ActorToIgnore;
	ActorToIgnore.Add(this);
	ActorToIgnore.Add(GetOwner());
	CouchProjectile->Initialize(LaunchVelocity, ActorToIgnore);
	CouchFishingRod = FishingRod;
}

#pragma endregion

void ACouchLure::OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CouchProjectile)
	{
		if (CouchProjectile->GetCanMove())
		{
			CouchProjectile->SetCanMove(false);
			SphereComponent->SetSimulatePhysics(true);

			const FVector Vel = FVector::ZeroVector;
			SphereComponent->SetPhysicsLinearVelocity(Vel);
			SphereComponent->SetPhysicsAngularVelocityInDegrees(Vel);
		}	
	}
	
	if (OtherActor->Implements<UCouchPickable>())
	{
		FishingObject = Cast<ACouchPickableMaster>(OtherActor);
		if (FishingObject)
		{
			FishingObject->PhysicsCollider->SetSimulatePhysics(false);
			FishingObject->PhysicsCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FishingObject->AttachToComponent(LureMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);	
		}
	}
}

void ACouchLure::DestroyLure()
{
	if (FishingObject)
		FishingObject->Destroy();

	Destroy();
}

TSubclassOf<ACouchPickableMaster> ACouchLure::GetFishingObject()
{
	if (FishingObject)
	{
		return FishingObject.GetClass();	
	}
	return nullptr;
}



