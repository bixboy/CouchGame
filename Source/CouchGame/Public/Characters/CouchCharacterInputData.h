// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CouchCharacterInputData.generated.h"

class UInputAction;
class EnhancedInputSubsystems;

/**
 * 
 */
UCLASS()
class COUCHGAME_API UCouchCharacterInputData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMove;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionDash;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionInteract;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionFire;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputFishingRod;
};
