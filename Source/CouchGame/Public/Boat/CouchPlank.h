// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CouchWidgetSpawn.h"
#include "GameFramework/Actor.h"
#include "CouchPlank.generated.h"

class UBoxComponent;
class UCouchWidgetSpawn;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USceneComponent;
class UNiagaraComponent;

UCLASS()
class COUCHGAME_API ACouchPlank : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ACouchPlank();

	~ACouchPlank();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UObject> InterractWidget;

	UPROPERTY()
	UStaticMeshComponent* PlankMesh;

	UStaticMesh* GetRandomStaticMesh();
	
	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = "AAA_PlankMeshes", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMesh*> DamagedPlanckMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BlockAllBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						   bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* WaterParticle;

	UPROPERTY(EditAnywhere)
	USceneComponent* WidgetPos;

	UPROPERTY(EditAnywhere)
	UCouchWidgetSpawn* CouchWidgetSpawn;
	
	UPROPERTY(VisibleAnywhere)
	bool IsInInterractZone;
	
};
