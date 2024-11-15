// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchProjectileExecuteTime.h"
#include "GameFramework/Actor.h"
#include "CouchProjectileEffect.generated.h"


class ACouchPickableCannonBall;
class ACouchCannonBall;

UCLASS()
class COUCHGAME_API ACouchProjectileEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchProjectileEffect();
	virtual void ExecuteEffect();
	
	UPROPERTY(EditAnywhere)
	ECouchProjectileExecuteTime ExecuteTime = ECouchProjectileExecuteTime::OnImpactWithHit;

	float GetDelay() const;

	void Init(ACouchCannonBall* InBall);
	void Init(ACouchPickableCannonBall* InBall);
	void Init(AActor* InActor);
	void Init(FHitResult InHitResult);

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	UPROPERTY()
	TObjectPtr<ACouchCannonBall> CouchCannonBall;
	UPROPERTY()
	TObjectPtr<ACouchPickableCannonBall> PickableCannonBall;
	UPROPERTY()
	TObjectPtr<AActor> ActorToInterractWith;
	UPROPERTY()
	FHitResult HitResult;
	bool HasExecutedEffect;
	
private:
	UPROPERTY(EditAnywhere)
	float Delay = 5.0f;
	float Timer;

	


};
