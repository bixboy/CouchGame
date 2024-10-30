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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchChargePower* ChargePower;

private:

	UPROPERTY()
	UCableComponent* Cable;
	UPROPERTY()
	float CableScale = 1.0f;
	UPROPERTY()
	UMaterial* CableMaterial;

	UPROPERTY(EditAnywhere)
	UClass* Lure;
	UPROPERTY()
	ACouchLure* LureRef;

	UPROPERTY(EditAnywhere)
	float Threshold;
	UPROPERTY(EditAnywhere)
	float RewindSpeed;

	UPROPERTY(EditAnywhere)
	float MaxCableLength;
	UPROPERTY(EditAnywhere)
	float MinCableLength;
	
	UFUNCTION()
	void SpawnLure();
	UFUNCTION()
	void InitializeCableAndConstraint();
	UFUNCTION()
	void RewindCable(float JoystickInput);
};
