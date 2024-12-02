// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "CouchCharacterMenu.generated.h"

class UCouchCharacterInputData;
class UInputMappingContext;

UCLASS()
class COUCHGAME_API ACouchCharacterMenu : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACouchCharacterMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma region Input

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<UCouchCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;
#pragma endregion 
};
