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

	void Init(ACouchCannonBall* InBall, ACouchPickableCannonBall* InPickBall = nullptr,AActor* InActor = nullptr, FHitResult InHitResult = FHitResult());
	bool IsExecutingOnDelay() const;
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "IsExecutingOnDelay()"))
	float Delay = 5.0f;
	
private:
	
	
	float Timer;

	


};
