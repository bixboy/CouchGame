// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchPickableMaster.h"

#include "Components/BoxComponent.h"
#include "Crafting/CouchCraftingTable.h"
#include "CouchLure.h"
#include "Misc/OutputDeviceNull.h"
#include "Widget/CouchWidgetSpawn.h"
#include "Widget/CouchWidget3D.h"

#pragma region Unreal Default
ACouchPickableMaster::ACouchPickableMaster()
{
	PrimaryActorTick.bCanEverTick = true;
	CouchProjectile = CreateDefaultSubobject<UCouchProjectile>(TEXT("ProjectileComponent"));
	WidgetSpawner = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("WidgetSpawner"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	PhysicsCollider = Mesh;
	RootComponent = Mesh;
	
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(Mesh);
	QteWidgetPose = CreateDefaultSubobject<USceneComponent>(TEXT("QteWidgetPose"));

}

void ACouchPickableMaster::BeginPlay()
{
	Super::BeginPlay();
	// PhysicsCollider = GetComponentByClass<UStaticMeshComponent>();
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
	CurrentPlayer->AnimationManager->IsCarryingItem = true;
	
	PhysicsCollider->SetSimulatePhysics(false);
	SetActorEnableCollision(false);
	
	AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("GripAttachObject")));
	
	if (CraftingTable)
	{
		CraftingTable->RemoveIngredient(this);
		CraftingTable = nullptr;
	}

}

void ACouchPickableMaster::Drop_Implementation()
{
	if (!PhysicsCollider) return;
	if (CurrentPlayer) CurrentPlayer->AnimationManager->IsCarryingItem = false;
	
	ICouchPickable::Drop_Implementation();
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (!CraftingTable) PhysicsCollider->SetSimulatePhysics(true);
	SetActorEnableCollision(true);

}

void ACouchPickableMaster::InteractWithObject_Implementation(ACouchInteractableMaster* interactable)
{
	ICouchPickable::InteractWithObject_Implementation(interactable);
	
	if (interactable->IsA(ACouchCraftingTable::StaticClass()))
	{
		ACouchCraftingTable* CraftTable = Cast<ACouchCraftingTable>(interactable);
		if (!CraftTable || CraftTable->IsCraftingTableFull()) return;
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsCarryingItem = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Interact with CraftingTable"));
		CraftTable->AddIngredient(this);
		CraftingTable = CraftTable;
	}
}

bool ACouchPickableMaster::CanInteractWith(TObjectPtr<ACouchInteractableMaster> Interactable) const
{
	if (ClassesPickableItemCanInteractWith.Contains(Interactable.GetClass())) return true;
	return false;
}

#pragma region Attach Lure

bool ACouchPickableMaster::AttachLure(TObjectPtr<ACouchLure> LureRef)
{
	if (CurrentLuresAttached.Num() <= 2)
	{
		CurrentLuresAttached.Add(LureRef);
		if (CurrentLuresAttached.Num() == 2)
		{
			CurrentPercentQte = 0.5f;
			InitQte();
			return true;
		}
		return false;
	}
	if (CurrentLuresAttached.Num() == 2)
	{
		CurrentPercentQte = 0.5f;
		InitQte();
		return true;
	}
	return false;
}

void ACouchPickableMaster::Detachlure(TObjectPtr<ACouchLure> LureRef)
{
	CurrentLuresAttached.Remove(LureRef);
	if (CurrentLuresAttached.Num() == 0)
	{
		PhysicsCollider->SetSimulatePhysics(true);
	}
}

#pragma endregion

#pragma region Qte Rewind

void ACouchPickableMaster::InitQte()
{
	// Position & Spawn Widget
	QteWidgetPose->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50.f));
	WidgetSpawner->SpawnWidget(WidgetQte, QteWidgetPose);

	// Init Widget
	if (WidgetSpawner->GetCurrentWidget())
	{
		FOutputDeviceNull ar;
		FString CmdAndParams = FString::Printf(TEXT("Init %s"), *this->GetName());
		WidgetSpawner->GetCurrentWidget()->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);
	}

	// Start QTE
	for (TObjectPtr<ACouchLure> LuresAttached : CurrentLuresAttached)
	{
		LuresAttached->CouchFishingRod->StartQte();
	}
}

void ACouchPickableMaster::UpdatePercent(float Value)
{
	CurrentPercentQte += Value;
	CurrentPercentQte = FMath::Clamp(CurrentPercentQte, 0.0f, 1.0f);
	
	if (CurrentPercentQte >= 1.f || CurrentPercentQte <= 0.f)
		StopQte();
}

float ACouchPickableMaster::GetQtePercent() const
{
	return CurrentPercentQte;
}

void ACouchPickableMaster::StopQte()
{
	if(WidgetSpawner->GetCurrentWidget())
	{
		WidgetSpawner->DestroyWidget();
		for (TObjectPtr<ACouchLure> LuresAttached : CurrentLuresAttached)
		{
			LuresAttached->CouchFishingRod->StopQte();

			// Si Team 1 Gagne
			if (LuresAttached->CouchFishingRod->GetTeam() == 2 && CurrentPercentQte >= 1)
			{
				LuresAttached->CouchFishingRod->DestroyFishingRod();
			}

			// Si Team 2 Gagne
			if (LuresAttached->CouchFishingRod->GetTeam() == 1 && CurrentPercentQte <= 0)
			{
				LuresAttached->CouchFishingRod->DestroyFishingRod();
			}
		}
	}
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


