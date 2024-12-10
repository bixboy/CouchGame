// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileEffect/CouchProjectileEffect.h"
#include "CouchProjectileEffectMud.generated.h"

class AMud;

UCLASS()
class COUCHGAME_API ACouchProjectileEffectMud : public ACouchProjectileEffect
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchProjectileEffectMud();
	
	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteEffect() override;

private:
UPROPERTY(EditAnywhere)
TSubclassOf<AMud> Mud;

UPROPERTY()
TObjectPtr<AMud> MudPtr;

UPROPERTY(EditAnywhere)
float DelayToExecute = 5.f;
float Timer;

UPROPERTY(EditAnywhere)
float PlayerSlowingSpeed;
UPROPERTY(EditAnywhere)
FVector2f ItemsSlowingSpeed;
	

};
