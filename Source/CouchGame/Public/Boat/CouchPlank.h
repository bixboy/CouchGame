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

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	UBoxComponent* BlockAll;
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	UClass* InteractWidget;

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

	UPROPERTY()
	ABoatFloor* Floor; 
	UPROPERTY(EditAnywhere)
	float TimeToRepair;
	float Timer;
	UPROPERTY()
	ACouchCharacter* APlayer;
};
