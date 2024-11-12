// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchStaticCanonBall.generated.h"
class UStaticMeshComponent;
class ACouchPickableCannonBall;
UCLASS()
class COUCHGAME_API ACouchStaticCanonBall : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchStaticCanonBall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Top;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Down;

	void InitCanonBall(TObjectPtr<ACouchPickableCannonBall> PickableCannonBall);
};

