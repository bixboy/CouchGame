// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchPickableMaster.h"

#include "Misc/OutputDeviceNull.h"
#include "Widget/CouchWidgetSpawn.h"
#include "Widget/CouchWidget3D.h"

#pragma region Unreal Default
ACouchPickableMaster::ACouchPickableMaster()
{
	PrimaryActorTick.bCanEverTick = true;
	CouchProjectile = CreateDefaultSubobject<UCouchProjectile>(TEXT("ProjectileComponent"));
	WidgetSpawner = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("WidgetSpawner"));
}

void ACouchPickableMaster::BeginPlay()
{
	Super::BeginPlay();
	PhysicsCollider = GetComponentByClass<UStaticMeshComponent>();
}

#pragma endregion

void ACouchPickableMaster::Interact_Implementation(ACouchCharacter* Player)
{
	if (!Player) return;
	Super::Interact_Implementation(Player);
	SetPlayerIsIn(!Execute_IsUsedByPlayer(this)); // Je toggle le bPlayerIsIn
	if (Execute_IsUsedByPlayer(this))
	{
		Execute_PickUp(this, Player);
	}
	else
	{
		Execute_Drop(this);
	}
}

void ACouchPickableMaster::PickUp_Implementation(ACouchCharacter* player)
{
	ICouchPickable::PickUp_Implementation(player);
	if (!PhysicsCollider) return;
	PhysicsCollider->SetSimulatePhysics(false);
	AttachToComponent(player->GetMesh(), FAttachmentTransformRules::KeepWorldTransform);
	FVector ItemLocation = player->PickUpItemPosition->GetComponentLocation();
	SetActorLocation(ItemLocation);

}

void ACouchPickableMaster::Drop_Implementation()
{
	if (!PhysicsCollider) return;
	ICouchPickable::Drop_Implementation();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	PhysicsCollider->SetSimulatePhysics(true);
}

void ACouchPickableMaster::InteractWithObject_Implementation(ACouchInteractableMaster* interactable)
{
	ICouchPickable::InteractWithObject_Implementation(interactable);
}

bool ACouchPickableMaster::CanInteractWith(TObjectPtr<ACouchInteractableMaster> Interactable) const
{
	if (ClassesPickableItemCanInteractWith.Contains(Interactable.GetClass())) return true;
	return false;
}

#pragma region Qte Rewind

bool ACouchPickableMaster::AttachLure(TObjectPtr<ACouchLure> LureRef)
{
	if (CurrentLuresAttached.Num() < 2)
	{
		CurrentLuresAttached.Add(LureRef);
		return false;
	}
	else if (CurrentLuresAttached.Num() == 2)
	{
		if (WidgetSpawner->GetCurrentWidget())
		{
			FOutputDeviceNull ar;
			FString CmdAndParams = FString::Printf(TEXT("Init"));
			WidgetSpawner->GetCurrentWidget()->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);	
		}
		
		CurrentPercentQte = 0.f;
		return true;
	}
	return false;
}

void ACouchPickableMaster::Detachlure(TObjectPtr<ACouchLure> LureRef)
{
	if (CurrentLuresAttached.Find(LureRef))
	{
		CurrentLuresAttached.Remove(LureRef);
		if (CurrentLuresAttached.Num() == 0)
		{
			PhysicsCollider->SetSimulatePhysics(true);
		}
	}
}

void ACouchPickableMaster::UpdatePercent(float Value)
{
	CurrentPercentQte += Value;
}

float ACouchPickableMaster::GetQtePercent() const
{
	return CurrentPercentQte;
}

#pragma endregion

TObjectPtr<ACouchInteractableMaster> ACouchPickableMaster::PlayerCanUsePickableItemToInteract(
	TObjectPtr<ACouchInteractableMaster>  PickableItem,
	TArray<TObjectPtr<ACouchInteractableMaster>> InteractableActors
	)
{
	InteractableActors.Remove(PickableItem);
	for (auto Interactable : InteractableActors)
	{
		if (CanInteractWith(Interactable))
		{
			return Interactable;
		}
	}
	return nullptr;
}


