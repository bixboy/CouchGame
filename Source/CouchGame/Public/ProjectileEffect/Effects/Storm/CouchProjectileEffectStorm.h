// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect/CouchProjectileEffect.h"
#include "CouchProjectileEffectStorm.generated.h"

class AStorm;

UCLASS()
class COUCHGAME_API ACouchProjectileEffectStorm : public ACouchProjectileEffect
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchProjectileEffectStorm();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void ExecuteEffect() override;

	UFUNCTION()
	void OnActorOverlapStorm(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStorm> Storm;

	UPROPERTY()
	TObjectPtr<AStorm> StormPtr;
	
	UPROPERTY(EditAnywhere)
	float DelayToExecute = 5.0;
	float Timer;
	
	UPROPERTY(EditAnywhere)
	float StormStrength;

	void StormEffect(AActor* Actor) const;


};
