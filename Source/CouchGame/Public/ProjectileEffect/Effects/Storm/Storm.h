// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Storm.generated.h"

UCLASS()
class COUCHGAME_API AStorm : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStorm();
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	

};
