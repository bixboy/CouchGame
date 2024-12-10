// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "CouchInteractableMaster.generated.h"

UCLASS()
class COUCHGAME_API ACouchInteractableMaster : public AActor, public ICouchInteractable
{
	GENERATED_BODY()

public:
	ACouchInteractableMaster();

	virtual bool IsUsedByPlayer_Implementation() override;
	virtual void Interact_Implementation(ACouchCharacter* Player) override;

	UFUNCTION(BlueprintCallable)
	ACouchCharacter* GetCurrentPlayer() const;
	UFUNCTION()
	void SetCurrentPlayer(ACouchCharacter* Player);
	UFUNCTION()
	void RemoveCurrentPlayer();

	UFUNCTION()
	void SetPlayerIsIn(bool IsIn);


protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACouchCharacter> CurrentPlayer;
	UPROPERTY()
	bool PlayerIsIn = false;
	
};
