// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchInteractableMaster.h"
#include "Components/CouchProjectile.h"
#include "Interfaces/CouchPickable.h"
#include "CouchPickableMaster.generated.h"


UCLASS()
class COUCHGAME_API ACouchPickableMaster : public ACouchInteractableMaster, public ICouchPickable
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	ACouchPickableMaster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchProjectile> CouchProjectile;

protected:
	virtual void BeginPlay() override;

#pragma endregion

#pragma region Interactables
public:
	virtual void Interact_Implementation(ACouchCharacter* Player) override;
	
#pragma endregion
#pragma region CouchPickable
public:
	virtual void PickUp_Implementation(ACouchCharacter* player) override;

	virtual void Drop_Implementation() override;

	virtual void InteractWithObject_Implementation(ACouchInteractableMaster* interactable) override;

private:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> PhysicsCollider;
#pragma endregion
#pragma region Interact with other Class
public:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ACouchInteractableMaster>> ClassesPickableItemCanInteractWith;

	TObjectPtr<ACouchInteractableMaster> PlayerCanUsePickableItemToInteract(
		TObjectPtr<ACouchInteractableMaster>  PickableItem, 
		TArray<TObjectPtr<ACouchInteractableMaster>> InteractableActors
	);

private:
	bool CanInteractWith(TObjectPtr<ACouchInteractableMaster> Interactable) const;
#pragma endregion
	
};
