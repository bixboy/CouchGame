#pragma once

#include "CoreMinimal.h"
#include "Characters/CouchCharacter.h"
#include "UObject/Interface.h"
#include "CouchInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCouchInteractable : public UInterface
{
	GENERATED_BODY()
};

class COUCHGAME_API ICouchInteractable
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(ACouchCharacter* Player);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartMoveActor(int InputDirection);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopMoveActor();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StartChargeActor();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StopChargeActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsUsedByPlayer();
};

