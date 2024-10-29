// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CouchProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "CouchLure.generated.h"

UCLASS()
class COUCHGAME_API ACouchLure : public AActor
{
	GENERATED_BODY()

public:
	ACouchLure();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCouchProjectile* CouchProjectile;
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LureMesh;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
