// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CouchPickable.generated.h"

class ACouchCharacter;
// This class does not need to be modified.
UINTERFACE()
class UCouchPickable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME_API ICouchPickable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PickUp(ACouchCharacter* player);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Drop();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractWithObject(ACouchInteractableMaster* interactable);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsPickable();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsPickable(bool isPickable);
	
	
};
