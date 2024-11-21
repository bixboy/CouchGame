// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Fog.generated.h"

UCLASS()
class COUCHGAME_API AFog : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFog();
	virtual void Tick(float DeltaTime) override;
	void UpdateFog(float Value, float MaxOpacity) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	
};
