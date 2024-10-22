// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CouchCharacter.generated.h"

class UCouchCharacterStateMachine;
UCLASS()
class COUCHGAME_API ACouchCharacter : public ACharacter
{
	GENERATED_BODY()
#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ACouchCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion
#pragma region Orient
public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.;

	void RotateMeshUsingOrientX() const;

#pragma endregion
#pragma region State Machine
public:
	void CreateStateMachine();

	void InitStateMachine();

protected:
	UPROPERTY()
	TObjectPtr<UCouchCharacterStateMachine> StateMachine;
#pragma endregion
};
