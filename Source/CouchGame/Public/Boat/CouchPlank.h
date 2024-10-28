// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CouchWidgetSpawn.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "CouchPlank.generated.h"

class ABoatFloor;
class UBoxComponent;
class UCouchWidgetSpawn;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USceneComponent;
class UNiagaraComponent;

UCLASS()
class COUCHGAME_API ACouchPlank : public AActor, public ICouchInteractable
{
	GENERATED_BODY()

public:
	ACouchPlank();

	void Init(ABoatFloor* floor);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	UBoxComponent* BlockAll;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UObject> InterractWidget;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HitMesh;

	UStaticMesh* GetRandomStaticMesh();
	
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = "AAA_PlankMeshes", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMesh*> DamagedPlanckMeshes;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InterractiveBoxRange;
	
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WaterParticle;

	UPROPERTY(EditAnywhere)
	USceneComponent* WidgetPos;

	UPROPERTY(EditAnywhere)
	UCouchWidgetSpawn* CouchWidgetSpawn;

	virtual void Interact_Implementation(ACouchCharacter* Player) override;
	bool IsPlayerRepairing;

	ABoatFloor* Floor; 
	UPROPERTY(EditAnywhere)
	float TimeToRepair;
	float Timer;
};
