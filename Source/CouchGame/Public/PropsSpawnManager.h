// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Fishable.h"
#include "UObject/Object.h"
#include "PropsSpawnManager.generated.h"
UCLASS()
class COUCHGAME_API APropsSpawnManager : public AActor
{
	GENERATED_BODY()

public:
	explicit APropsSpawnManager();

	UFUNCTION(BlueprintCallable)
	int GetRandomIndex();

	TScriptInterface<IFishable> GetPropsToSpawn(int Index);

	UFUNCTION(BlueprintCallable)
	void SpawnProps(FTransform Transform);

	UFUNCTION(BlueprintCallable)
	void AddFishableToArray(TScriptInterface<IFishable> FishableObject);

	UPROPERTY(BlueprintReadWrite)
	TArray<TScriptInterface<IFishable>> FishableArray;
};
