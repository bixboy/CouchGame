// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/CouchInteractableMaster.h"
#include "CouchCraftingValidateItem.generated.h"

class ACouchCraftingTable;

UCLASS()
class COUCHGAME_API ACouchCraftingValidateItem : public ACouchInteractableMaster
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchCraftingValidateItem();

	virtual void Interact_Implementation(ACouchCharacter* Player) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ACouchCraftingTable> CraftingTable;

	

	
};
