// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchPickableCannonBall.h"
#include "InputAction.h"
#include "Components/CouchMovement.h"
#include "Widget/CouchWidgetSpawn.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "CoucheCannon.generated.h"


UCLASS()
class COUCHGAME_API ACoucheCannon : public AActor, public ICouchInteractable
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY()
	bool PlayerIsIn = false;

	UPROPERTY()
	ACouchCharacter* CurrentPlayer;

	UFUNCTION()
	void SetupCannon();

#pragma region Shoot
private:
	UPROPERTY()
	FVector TargetLocation;
	
	UPROPERTY()
	bool CanShoot = false;

	UPROPERTY(EditAnywhere)
	float ArcShoot = 0.7f;
	
	UPROPERTY()
	float AttackRange;

	UFUNCTION()
	void SpawnBullet();
	UFUNCTION()
	FVector LineTrace();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UClass* Bullet;
	
	UFUNCTION()
	void Fire() {SpawnBullet();}
	
#pragma endregion

#pragma region Power Charge

public:
	UFUNCTION(BlueprintCallable)
	void StartCharging();
	UFUNCTION(BlueprintCallable)
	void StopCharging();
private:	
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
	UPROPERTY()
	bool IsInCharge = false;

	UFUNCTION()
	void SetupTimeLine();

	UFUNCTION()
	void UpdatePower(float Alpha);
	
#pragma endregion

#pragma region Reload
private:
	UPROPERTY()
	ACouchPickableCannonBall* AmmoActor;

	UPROPERTY()
	int CurrentAmmo;

public:
	UFUNCTION(BlueprintCallable)
	void Reload(ACouchPickableCannonBall* CannonBallReload);
	
#pragma endregion	

#pragma region Movement
private:
	
	UFUNCTION(BlueprintCallable)
	void StartMovement(int InputDirection);
	UFUNCTION(BlueprintCallable)
	void StopMovement();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchMovement* MovementComponent;
	
#pragma endregion	

#pragma region Widget
private:
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	UClass* PowerChargeWidget;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchWidgetSpawn* WidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* WidgetPose;
	
#pragma endregion

#pragma region Animation
private:
	UPROPERTY(EditDefaultsOnly, Category = DefaultValue)
	UAnimMontage* ShootAnimation;
	
#pragma endregion	
	
protected:
	virtual void BeginPlay() override;

public:
	ACoucheCannon();
	
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(ACouchCharacter* Player) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* PlayerPose;
	
};
