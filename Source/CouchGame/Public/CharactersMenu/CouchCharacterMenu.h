// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "CouchCharacterMenu.generated.h"

struct FInputActionValue;
class UCouchCharacterLobby;
class UCouchCharacterInputData;
class UInputMappingContext;

UCLASS()
class COUCHGAME_API ACouchCharacterMenu : public APawn
{
	GENERATED_BODY()
#pragma region Unreal Default
public:
	// Sets default values for this actor's properties
	ACouchCharacterMenu();

	UFUNCTION(BlueprintCallable)
	void InitMenuCharacter(UCouchCharacterLobby* In_Lobby);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY()
	APlayerController* PlayerController;
#pragma endregion
	
#pragma region Input / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<UCouchCharacterInputData> InputData;
	
	void SetupMappingContextIntoController() const;

#pragma endregion
#pragma region Actions

	
	void BindInputActions(UEnhancedInputComponent* EnhancedInputComponent);
	int GetPlayerIndex() const;
	void OnInputNavigate(const FInputActionValue& InputActionValue);
	void OnInputValidate();
	void OnInputCancel();

	UPROPERTY()
	TObjectPtr<UCouchCharacterLobby> Lobby;

#pragma endregion
};
