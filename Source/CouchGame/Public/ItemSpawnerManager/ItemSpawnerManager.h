#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnerManager.generated.h"

class UBoxComponent;
class ACouchPickableMaster;

UCLASS()
class COUCHGAME_API AItemSpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnItemBeginOverlapDestoryBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult);
	// Sets default values for this actor's properties
	AItemSpawnerManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyItem(ACouchPickableMaster* ItemToDestroy, bool isDestroyByItemSpawnerManager = true);

private:
	void SpawnItem();

	TSubclassOf<ACouchPickableMaster> RandomSpawn();

	// TSubclassOf<ACouchPickableMaster> LoopSpawn();
	//
	// TSubclassOf<ACouchPickableMaster> PingPongSpawn();

	// int Iterator = 0;
	// int PingPongDirection = 1;
	
	UPROPERTY(EditAnywhere)
	TMap<TSubclassOf<ACouchPickableMaster>, float> ItemsToSpawn;

	UPROPERTY(EditAnywhere)
	float SpawnRate = 1.0f;
	float Timer = 0.0f;

	UPROPERTY(EditAnywhere, Category = ItemSpawner, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> SpawnBox;

	UPROPERTY(EditAnywhere, Category = ItemSpawner, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> DestroyBox;

	UPROPERTY(EditAnywhere)
	int MaxItemSpawnedAtSameTime;

	TArray<TObjectPtr<ACouchPickableMaster>> CurrentSpawnedItems;
	
};


