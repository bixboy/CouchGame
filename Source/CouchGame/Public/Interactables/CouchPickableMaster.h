// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchInteractableMaster.h"
#include "Components/CouchProjectile.h"
#include "Interfaces/CouchPickable.h"
#include "CouchPickableMaster.generated.h"

class ACouchWidget3D;
class ACouchCraftingTable;
class ACouchLure;
class UCouchWidgetSpawn;
class UCouchInteractableMaster;

UCLASS()
class COUCHGAME_API ACouchPickableMaster : public ACouchInteractableMaster, public ICouchPickable
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	ACouchPickableMaster();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> InteractionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchProjectile> CouchProjectile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchWidgetSpawn> WidgetSpawner;

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

	virtual bool IsPickable_Implementation() override;

	virtual void SetIsPickable_Implementation(bool isPickable) override;
	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> PhysicsCollider;

	bool IsPickable = true;
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

#pragma region Fishing
public:
	bool AttachLure(TObjectPtr<ACouchLure> LureRef);
	void Detachlure(TObjectPtr<ACouchLure> LureRef);
	void UpdatePercent(float Value);
	void StopQte();

	UFUNCTION(BlueprintCallable)
	float GetQtePercent() const;

	UFUNCTION(meta = (MapParam))
	bool GetTeamAttached(int Team);

	TArray<TObjectPtr<ACouchLure>> GetLuresRef() const;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> QteWidgetPose;
	
	void InitQte();
	
	UPROPERTY()
	TArray<TObjectPtr<ACouchLure>> CurrentLuresAttached;

	UPROPERTY(EditAnywhere, Category = DefaultsValue)
	TSubclassOf<ACouchWidget3D> WidgetQte;

	UPROPERTY()
	float CurrentPercentQte = 0.5f;
	
#pragma endregion	

#pragma region Crafting
	TObjectPtr<ACouchCraftingTable> CraftingTable;
	
#pragma endregion
};
