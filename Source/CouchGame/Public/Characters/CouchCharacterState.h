// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CouchCharacterState.generated.h"

class UCouchCharacterStateMachine;
class ACouchCharacter;
enum class ECouchCharacterStateID : uint8;

UCLASS(Abstract)
class COUCHGAME_API UCouchCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCouchCharacterState();

	virtual ECouchCharacterStateID GetStateID();

	virtual void StateInit(UCouchCharacterStateMachine* InStateMachine);

	virtual void StateEnter(ECouchCharacterStateID PreviousStateID);

	virtual void StateExit(ECouchCharacterStateID NextStateID);

protected:
	UPROPERTY()
	TObjectPtr<ACouchCharacter> Character;

	UPROPERTY()
	TObjectPtr<UCouchCharacterStateMachine> StateMachine;
};


