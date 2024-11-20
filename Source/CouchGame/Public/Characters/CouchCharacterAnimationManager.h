// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CouchCharacterAnimationManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class COUCHGAME_API UCouchCharacterAnimationManager : public UObject
{
	GENERATED_BODY()
public:

	// États accessibles en lecture dans les Blueprints
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsRunning;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsCarryingItem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsFishing;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsFishingStart;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsFishingRelease;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsFishingPull;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsDragging;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsDraggingForward;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsDraggingBackward;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsInteractCatapultWithAmmo;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsChargingCatapult;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsReleasingCatapult;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsCheckingChef;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool HasPressedDashInput;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsDashCooldown;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsRepairing;

	
};

