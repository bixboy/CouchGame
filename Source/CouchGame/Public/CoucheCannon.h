// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchWidgetSpawn.h"
#include "Components/SplineComponent.h"
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

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	bool CanShoot = false;
	UPROPERTY()
	float AttackRange;

	void SpawnBullet();
	FVector LineTrace();

#pragma region Movement

	UPROPERTY()
	FTimeline MoveTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* MoveCurve;
	UPROPERTY()
	USplineComponent* LinePathComponent;

	UFUNCTION(BlueprintCallable)
	void StartMovement(int InputDirection);
	UFUNCTION(BlueprintCallable)
	void StopMovement();
	UFUNCTION()
	void MoveCannon(float Alpha);

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float SpeedMovement;
	
#pragma endregion	
	
#pragma region Power

	UPROPERTY()
	FTimeline PowerTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* PowerCurve;
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float MaxPower = 500.f;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float MinPower = 100.f;
	UPROPERTY()
	float CurrentPower;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float SpeedCharge = 1.f;

	UFUNCTION()
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
	FName StartPointName = "barrel";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	FName WidgetPoseName = "WidgetPose";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UClass* Bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	AActor* LinePath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchWidgetSpawn* WidgetComponent;
	
};
