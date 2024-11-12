// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/CouchInteractableMaster.h"
#include "CouchCraftingTable.generated.h"

UCLASS()
class COUCHGAME_API ACouchCraftingTable : public ACouchInteractableMaster
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	// Sets default values for this actor's properties
	ACouchCraftingTable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation(ACouchCharacter* Player) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Table;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Plate1Position;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Plate1Box;

	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Plate2Position;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Plate2Box;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> FillBarPosition;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> PlateSuggestionPos;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> FinalDishSpawnPosition;
#pragma endregion

#pragma region Crafting

	
#pragma endregion
};
