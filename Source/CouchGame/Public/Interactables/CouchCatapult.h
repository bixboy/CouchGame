// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchInteractableWeapons.h"
#include "CouchPickableCannonBall.h"
#include "Components/CouchChargePower.h"
#include "CouchCatapult.generated.h"

UCLASS()
class COUCHGAME_API ACouchCatapult : public ACouchInteractableWeapons
{
	GENERATED_BODY()

public:
	ACouchCatapult();

	virtual void StartChargeActor_Implementation() override;
	virtual void StopChargeActor_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchChargePower* PowerChargeComponent;
	
private:
	UFUNCTION()
	void SpawnBullet();
	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	bool IsInCharge = false;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float CurveShoot = 0.7f;
	
	UPROPERTY(EditDefaultsOnly, Category = DefaultValue)
	UAnimMontage* ShootAnimation;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	UClass* PowerChargeWidget;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UClass* Bullet;

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
};
