// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CouchCharacterLobby.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME_API UCouchCharacterLobby : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent,  meta = (DisplayName = "Move To Left"))
	void Receive_Left(int PlayerIndex = -1);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Move To Right"))
	void Receive_Right(int PlayerIndex = -1);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Move To Up"))
	void Receive_Up(int PlayerIndex = -1);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Move To Down"))
	void Receive_Down(int PlayerIndex = -1);

	UFUNCTION(BlueprintImplementableEvent,  meta = (DisplayName = "OnPlayerValidate"))
	void Receive_Validate(int PlayerIndex = -1);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerCancel"))
	void Receive_Cancel(int PlayerIndex = -1);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPlayerCancelCompleted"))
	void Receive_Cancel_Completed(int PlayerIndex = -1);

	
	
};
