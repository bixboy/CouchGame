// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/CouchMovement.h"
#include "Interactables/CouchInteractableMaster.h"
#include "Widget/CouchWidgetSpawn.h"
#include "CouchInteractableWeapons.generated.h"

UCLASS()
class COUCHGAME_API ACouchInteractableWeapons : public ACouchInteractableMaster
{
	GENERATED_BODY()

public:
	ACouchInteractableWeapons();
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(ACouchCharacter* Player) override;
	
	UFUNCTION()
	bool GetCanUse() const;
	UFUNCTION()
	void SetCanUse(bool Value);

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void Setup();
	
	UPROPERTY()
	bool CanUse = true;

#pragma region Setup
public:
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchMovement* MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PlayerPose;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchWidgetSpawn* WidgetComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* WidgetPose;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxInteract;
	
#pragma endregion

#pragma region Movement
private:	
	UFUNCTION()
	void StartMovement(int InputDirection);
	UFUNCTION()
	void StopMovement();
	
#pragma endregion	
};
