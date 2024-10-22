// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "CoucheCannon.generated.h"

UCLASS()
class COUCHGAME_API ACoucheCannon : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY()
	USceneComponent* StartPoint;
	FVector TargetLocation;

	void SpawnBullet();
	FVector LineTrace();
	
#pragma region Power
	
	UPROPERTY(EditAnywhere)
	UCurveFloat* PowerCurve;
	
	FTimeline PowerTimeline;
	
	float CurrentPower;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float MaxPower = 200.f;

	void SetupTimeLine();
	UFUNCTION(BlueprintCallable)
	void StartCharging();
	UFUNCTION(BlueprintCallable)
	void StopCharging();
	void UpdatePower(float Value);
	
#pragma endregion	

protected:
	virtual void BeginPlay() override;

public:
	ACoucheCannon();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	FName StartPointName = "barrel";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UClass* Bullet;

	bool CanShoot = false;
};
