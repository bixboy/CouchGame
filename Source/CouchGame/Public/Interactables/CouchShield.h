// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CouchMovement.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "Widget/CouchWidgetSpawn.h"
#include "CouchShield.generated.h"

UCLASS()
class COUCHGAME_API ACouchShield : public AActor, public ICouchInteractable
{
	GENERATED_BODY()

public:
	ACouchShield();

	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact_Implementation(FHitResult HitResult, ACouchCharacter* Player) override;

#pragma region Setup Shield
	
private:
	UFUNCTION()
	void SetupShield();

public:
	UPROPERTY(EditAnywhere)
	USceneComponent* PlayerPose;

	UPROPERTY(EditAnywhere)
	USceneComponent* WidgetPose;

	UPROPERTY(EditAnywhere)
	UCouchWidgetSpawn* WidgetComponent;
	
#pragma endregion

	
#pragma region Movement
private:
	UFUNCTION(BlueprintCallable)
	void StartMovement(int InputDirection);
	UFUNCTION(BlueprintCallable)
	void StopMovement();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCouchMovement* MovementComponent;
	
#pragma endregion

#pragma region Interact
	UPROPERTY()
	ACouchCharacter* CurrentPlayer;

	UPROPERTY()
	bool PlayerIsIn = false;
	
#pragma endregion	
	
};
