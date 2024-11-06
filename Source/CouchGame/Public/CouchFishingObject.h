// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchPickable.h"
#include "CouchFishingObject.generated.h"

UCLASS()
class COUCHGAME_API ACouchFishingObject : public AActor, public ICouchPickable
{
	GENERATED_BODY()

public:
	ACouchFishingObject();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CouchFishing)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CouchFishing)
	TObjectPtr<USphereComponent> Collider;
};
