// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchPlank.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchDamageable.h"
#include "BoatFloor.generated.h"

UCLASS()
class COUCHGAME_API ABoatFloor : public AActor, public ICouchDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoatFloor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Hit_Implementation(FHitResult HitResult) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* Collision;
	
	UPROPERTY(EditAnywhere)
	ACouchPlank* PlankHit;
	

};
