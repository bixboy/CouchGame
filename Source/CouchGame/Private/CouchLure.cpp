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

// Init Lure
void ACouchLure::Initialize(const FVector& LaunchVelocity, ACouchFishingRod* FishingRod)
{
	TArray<TObjectPtr<AActor>> ActorToIgnore;
	ActorToIgnore.Add(this);
	ActorToIgnore.Add(GetOwner());
	ActorToIgnore.Add(FishingRod);
	ActorToIgnore.Add(FishingRod->CurrentPlayer);
	CouchProjectile->Initialize(LaunchVelocity, ActorToIgnore, false);
	CouchFishingRod = FishingRod;
	CanAttach = true;
}

#pragma endregion

// Overlap Attach
void ACouchLure::OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Stop Movement
	if (CouchProjectile)
	{
		if (CouchProjectile->GetCanMove())
		{
			SphereComponent->SetSimulatePhysics(true);
			CouchProjectile->SetCanMove(false);
			
			const FVector Vel = FVector::ZeroVector;
			SphereComponent->SetPhysicsLinearVelocity(Vel);
			SphereComponent->SetPhysicsAngularVelocityInDegrees(Vel);
			
			CouchFishingRod->CurrentPlayer->AnimationManager->IsFishingRelease = false;
			CouchFishingRod->CurrentPlayer->AnimationManager->IsFishing = true;
		}	
	}

	// Attach Lure
	if (OtherActor->Implements<UCouchPickable>() && !FishingObject)
	{
		FishingObject = Cast<ACouchPickableMaster>(OtherActor);
		if (FishingObject && CouchFishingRod && !FishingObject->GetTeamAttached(CouchFishingRod->GetTeam()))
		{
			if (!FishingObject->AttachLure(this) && FishingObject->PhysicsCollider)
			{
				FishingObject->PhysicsCollider->SetSimulatePhysics(false);
				FishingObject->AttachToComponent(LureMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				if (CouchFishingRod && CouchFishingRod->GetCharacter() && CouchFishingRod->GetCharacter()->SpawnerManager)
				{
					CouchFishingRod->GetCharacter()->SpawnerManager->DestroyItem(FishingObject, false);
				}
				if (CouchFishingRod && CouchFishingRod->CurrentPlayer && CouchFishingRod->CurrentPlayer->AnimationManager)
				{
					CouchFishingRod->CurrentPlayer->AnimationManager->IsFishing = true;
				}
			}
			else
			{
				SphereComponent->SetSimulatePhysics(false);
			}
		}
	}
}

// Detach Object
void ACouchLure::DetachAttachObject()
{
	if (FishingObject)
	{
		FishingObject->Detachlure(this);
		FishingObject = nullptr;
	}
}

// Destoy Lure
void ACouchLure::DestroyLure()
{
	if (FishingObject)
		FishingObject->Destroy();

	Destroy();
}

#pragma region Getter

// Get Attached Object class
TSubclassOf<ACouchPickableMaster> ACouchLure::GetFishingObject() const
{
	if (FishingObject)
	{
		return FishingObject.GetClass();	
	}
	return nullptr;
}

// Get Attached Object Actor
ACouchPickableMaster* ACouchLure::GetFishingObjectActor() const
{
	if (FishingObject)
	{
		return FishingObject;
	}
	return nullptr;
}

#pragma endregion



