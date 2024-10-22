// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoucheCannon.generated.h"

UCLASS()
class COUCHGAME_API ACoucheCannon : public AActor
{
	GENERATED_BODY()

private:
	void Shoot();

	UPROPERTY()
	USceneComponent* StartPoint;

protected:
	virtual void BeginPlay() override;

public:
	ACoucheCannon();
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	FName StartPointName = "barrel";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	UClass* Bullet;
	
	FRotator LineTrace();
};
