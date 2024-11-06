// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/CouchPickableMaster.h"

#pragma region Unreal Default
// Sets default values
ACouchPickableMaster::ACouchPickableMaster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACouchPickableMaster::BeginPlay()
{
	Super::BeginPlay();
	PhysicsCollider = GetComponentByClass<UStaticMeshComponent>();
}

// Called every frame
void ACouchPickableMaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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


