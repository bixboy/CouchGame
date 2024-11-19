// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/CouchInteractableMaster.h"
#include "CouchCraftingValidateItem.generated.h"

class ACouchWidget3D;
class UCouchWidgetSpawn;
class ACouchCraftingTable;

UCLASS()
class COUCHGAME_API ACouchCraftingValidateItem : public ACouchInteractableMaster
{
	GENERATED_BODY()

protected:
	ACouchCraftingValidateItem();
	virtual void BeginPlay() override;

public:
	virtual void Interact_Implementation(ACouchCharacter* Player) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCouchWidgetSpawn> WidgetSpawner;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ACouchCraftingTable> CraftingTable;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> WidgetPose;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TSubclassOf<ACouchWidget3D> WidgetInteract;

	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
