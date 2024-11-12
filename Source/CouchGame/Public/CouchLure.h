#pragma once

#include "CoreMinimal.h"
#include "CouchFishingObject.h"
#include "Components/CouchProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interactables/CouchFishingRod.h"
#include "CouchLure.generated.h"

UCLASS()
class COUCHGAME_API ACouchLure : public AActor
{
	GENERATED_BODY()

public:
	ACouchLure();

#pragma region Defaults
public:	
	UFUNCTION(BlueprintCallable)
	void Initialize(const FVector& LaunchVelocity, ACouchFishingRod* FishingRod);

private:	
	UFUNCTION()
	void SetupLure();
	UFUNCTION()
	void OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	TObjectPtr<ACouchFishingRod> CouchFishingRod;
#pragma endregion	
	
#pragma region Components
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchProjectile> CouchProjectile;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> LureMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TopMesh;
#pragma endregion	

#pragma region Fishing Object
public:	
	UFUNCTION()
	TSubclassOf<ACouchPickableMaster> GetFishingObject();
	UFUNCTION()
	ACouchPickableMaster* GetFishingObjectActor();

private:
	UPROPERTY()
	TObjectPtr<ACouchPickableMaster> FishingObject;

#pragma endregion

public:
	UFUNCTION()
	void DestroyLure();
	UFUNCTION()
	void DetachObject();
	
};
