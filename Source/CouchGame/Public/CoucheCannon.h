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
	
	FTimeline PowerTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* PowerCurve;
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float MaxPower = 1.f;
	float CurrentPower;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float SpeedInterp = 1.f;

	void SetupTimeLine();
	UFUNCTION(BlueprintCallable)
	void StartCharging();
	UFUNCTION(BlueprintCallable)
	void StopCharging();
	UFUNCTION()
	void UpdatePower(float Alpha);
	
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
