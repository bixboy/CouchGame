// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Fishable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UFishable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME_API IFishable
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetProps();
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
