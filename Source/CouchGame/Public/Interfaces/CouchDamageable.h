// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CouchDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCouchDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME_API ICouchDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Hit(FHitResult HitResult);
};
