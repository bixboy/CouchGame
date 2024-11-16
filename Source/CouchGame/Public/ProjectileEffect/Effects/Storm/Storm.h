// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Storm.generated.h"

class URadialForceComponent;

UCLASS()
class COUCHGAME_API AStorm : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStorm();
	void Init(float StormStrength, float ForceRadius) const;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* RadialForceComponent;
	
	

};
