// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CouchCharacterStateMachine.generated.h"

class ACouchCharacter;
class UCouchCharacterState;
enum class ECouchCharacterStateID : uint8;
/**
 * 
 */
UCLASS()
class COUCHGAME_API UCouchCharacterStateMachine : public UObject
{
	GENERATED_BODY()
public:
	void Init(ACouchCharacter* InCharacter);

	ACouchCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(ECouchCharacterStateID NextStateID);

	UCouchCharacterState* GetState(ECouchCharacterStateID StateID);

protected:
	UPROPERTY()
	TObjectPtr<ACouchCharacter> Character;

	TArray<UCouchCharacterState*> AllStates;

	UPROPERTY(BlueprintReadOnly)
	ECouchCharacterStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<UCouchCharacterState> CurrentState;

	void FindStates();

	void InitStates();
	
	
};
