// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchFishingRod.generated.h"

UCLASS()
class COUCHGAME_API ACouchFishingRod : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchFishingRod();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
