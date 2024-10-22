// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CouchCharacterStateMachine.generated.h"

class ACouchCharacter;
class UCouchCharacterState;
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

protected:
	UPROPERTY()
	TObjectPtr<ACouchCharacter> Character;

	TArray<UCouchCharacterState*> AllStates;

	void FindStates();

	void InitStates();
	
	
};
