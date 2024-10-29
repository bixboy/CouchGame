// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CouchChargePower.h"
#include "CableComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CouchFishingRod.generated.h"

class ACouchLure;

UCLASS()
class COUCHGAME_API ACouchFishingRod : public AActor, public ICouchInteractable
{
	GENERATED_BODY()

public:
	ACouchFishingRod();
	virtual void Tick(float DeltaTime) override;

public:
	virtual void StartChargeActor_Implementation() override;
	virtual void StopChargeActor_Implementation() override;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* PhysicsConstraint;

private:
	UPROPERTY(EditAnywhere)
	UCouchChargePower* ChargePower;

	UPROPERTY()
	UCableComponent* Cable;
	UPROPERTY()
	float CableScale = 1.0f;
	UPROPERTY()
	UMaterial* CableMaterial;

	UPROPERTY()
	ACouchLure* Lure;
	
	UFUNCTION()
	void SpawnLure();
};
