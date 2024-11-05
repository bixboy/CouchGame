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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchProjectile> CouchProjectile;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> LureMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> TopMesh;

private:
	UFUNCTION()
	void OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void SetPhysics();

	FTimerHandle TimerHandle;
	FVector LastLocation;
};
