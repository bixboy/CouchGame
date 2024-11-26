// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchPlank.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchDamageable.h"
#include "BoatFloor.generated.h"

class ACouchBoat;

UCLASS()
class COUCHGAME_API ABoatFloor : public AActor, public ICouchDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoatFloor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init(ACouchBoat* Boat);

	virtual void Hit_Implementation(FHitResult HitResult, float RepairingTime = 0.f, float Scale = 0.f) override;

	virtual int GetBoatTeam_Implementation() override;

	void RemoveHitFromArray(ACouchPlank* Plank);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = "Boat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Collision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACouchPlank> FloorHit;

	TArray<ACouchPlank*> Hits;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat", meta = (AllowPrivateAccess = "true"))
	ACouchBoat* ABoat;

	UPROPERTY(EditAnywhere)
	float DamageFrequency = 1;
	float Timer;

	UPROPERTY(EditAnywhere)
	FVector PlankLocationOffset;

	UPROPERTY(EditAnywhere)
	int CurrentTeam;
};
