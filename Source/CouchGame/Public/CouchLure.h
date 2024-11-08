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

	UFUNCTION(BlueprintCallable)
	void Initialize(const FVector& LaunchVelocity, ACouchFishingRod* FishingRod);
	UFUNCTION()
	void DestroyLure();

	UFUNCTION()
	ACouchFishingObject* GetFishingObject();

private:
	UFUNCTION()
	void OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetupLure();

	UPROPERTY()
	TObjectPtr<ACouchFishingRod> CouchFishingRod;

	UPROPERTY()
	TObjectPtr<ACouchFishingObject> FishingObject;
	
};
