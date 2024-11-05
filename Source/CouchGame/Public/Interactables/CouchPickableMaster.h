// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchInteractableMaster.h"
#include "Interfaces/CouchPickable.h"
#include "CouchPickableMaster.generated.h"

UCLASS()
class COUCHGAME_API ACouchPickableMaster : public ACouchInteractableMaster, public ICouchPickable
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	// Sets default values for this actor's properties
	ACouchPickableMaster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region Interactables
public:
	virtual void Interact_Implementation(ACouchCharacter* Player) override;
	
#pragma endregion
#pragma region CouchPickable
public:
	virtual void PickUp_Implementation(ACouchCharacter* player) override;

	virtual void Drop_Implementation() override;

private:
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> PhysicsCollider;
#pragma endregion
	
};
