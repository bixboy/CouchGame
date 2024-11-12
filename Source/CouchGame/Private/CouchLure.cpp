#include "CouchLure.h"

#include "Components/CouchProjectile.h"
#include "Interactables/CouchInteractableMaster.h"
#include "Interactables/CouchPickableMaster.h"
#include "Interfaces/CouchPickable.h"
#include "ItemSpawnerManager/ItemSpawnerManager.h"

ACouchLure::ACouchLure()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupLure();
}

#pragma region Setup Lure

void ACouchLure::SetupLure()
{
	CouchProjectile = CreateDefaultSubobject<UCouchProjectile>(TEXT("ProjectileComponent"));
  
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
	// Stop Movement
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

	// Attach Lure
	if (OtherActor->Implements<UCouchPickable>() && !FishingObject)
	{
		FishingObject = Cast<ACouchPickableMaster>(OtherActor);
		if (FishingObject)
		{
			if (!FishingObject->AttachLure(this))
			{
				FishingObject->PhysicsCollider->SetSimulatePhysics(false);
				FishingObject->AttachToComponent(LureMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				CouchFishingRod->GetCharacter()->SpawnerManager->DestroyItem(FishingObject, false);	
			}
			else
			{
				SphereComponent->SetSimulatePhysics(false);
			}
		}
	}
}

void ACouchLure::DetachAttachedObject()
{
	if (FishingObject)
	{
		FishingObject->Detachlure(this);
		FishingObject = nullptr;
	}
}

void ACouchLure::DestroyLure()
{
	if (FishingObject)
		FishingObject->Destroy();

	Destroy();
}

#pragma region Getter

TSubclassOf<ACouchPickableMaster> ACouchLure::GetFishingObject() const
{
	if (FishingObject)
	{
		return FishingObject.GetClass();	
	}
	return nullptr;
}

ACouchPickableMaster* ACouchLure::GetFishingObjectActor() const
{
	if (FishingObject)
	{
		return FishingObject;
	}
	return nullptr;
}

#pragma endregion



