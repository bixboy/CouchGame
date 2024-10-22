// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CouchCharacterStateMachine.h"

#include "Characters/CouchCharacter.h"
#include "Characters/CouchCharactersStateID.h"
#include "Characters/CouchCharacterState.h"

void UCouchCharacterStateMachine::Init(ACouchCharacter* InCharacter)
{
	Character = InCharacter;
	FindStates();
	InitStates();
}

ACouchCharacter* UCouchCharacterStateMachine::GetCharacter() const
{
	return Character;
}

void UCouchCharacterStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents
	= Character->K2_GetComponentsByClass(UCouchCharacterState::StaticClass());
	for (UActorComponent* StateComponent : FoundComponents)
	{
		UCouchCharacterState* State = Cast<UCouchCharacterState>(StateComponent);
		if (!State) continue;
		if (State->GetStateID() == ECouchCharacterStateID::None) continue;

		AllStates.Add(State);
	}
}

void UCouchCharacterStateMachine::InitStates()
{
	for (UCouchCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}
