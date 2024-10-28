// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchPickableCannonBall.generated.h"

UCLASS()
class COUCHGAME_API ACouchPickableCannonBall : public AActor
{
	GENERATED_BODY()

public:
	ACouchPickableCannonBall();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UStaticMeshComponent* Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UStaticMeshComponent* Top;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UStaticMeshComponent* Down;

protected:
	virtual void BeginPlay() override;
	
};
