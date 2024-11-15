// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/CouchWidgetSpawn.h"
#include "Interactables/CouchInteractableMaster.h"
#include "Interfaces/CouchInteractable.h"
#include "CouchPlank.generated.h"

class ACouchWidget3D;
class ABoatFloor;
class UBoxComponent;
class UCouchWidgetSpawn;
class UStaticMeshComponent;
class UParticleSystemComponent;
class USceneComponent;
class UNiagaraComponent;

UCLASS()
class COUCHGAME_API ACouchPlank : public ACouchInteractableMaster
{
	GENERATED_BODY()

public:
	ACouchPlank();

	void Init(ABoatFloor* floor, float RepairingTime = 0.f, float Scale = 0.f);

	virtual bool IsUsedByPlayer_Implementation() override;

	// Overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Getter
	UFUNCTION(BlueprintCallable)
	float GetRepairingPercent() const;
	UFUNCTION(BlueprintCallable)
	AActor* GetInteractWidget() const;

#pragma region componenents
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<UCouchWidgetSpawn> CouchWidgetSpawn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> WaterParticle;
	
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> HitMesh;

	UPROPERTY(EditAnywhere, blueprintReadWrite)
	TObjectPtr<UBoxComponent> BlockAll;
	
	UPROPERTY(EditAnywhere, blueprintReadWrite)
	TObjectPtr<USceneComponent> WidgetPos;
	
	UPROPERTY(VisibleAnywhere, blueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> InterractiveBoxRange;

#pragma endregion	

protected:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<ACouchCharacter> APlayer;
	bool IsPlayerRepairing;

	// Widget
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TSubclassOf<ACouchWidget3D> InteractWidgetClass;
	UPROPERTY()
	TObjectPtr<AActor> InteractWidgetPtr;

	// Timer
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	float TimeToRepair;
	UPROPERTY()
	ABoatFloor* Floor;
	float Timer;

	virtual void Interact_Implementation(ACouchCharacter* Player) override;

	UStaticMesh* GetRandomStaticMesh();

	UPROPERTY(EditAnywhere,  BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UStaticMesh>> DamagedPlanckMeshes;
};
