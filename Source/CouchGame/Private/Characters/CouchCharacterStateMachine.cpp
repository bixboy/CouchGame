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

	ChangeState(ECouchCharacterStateID::Idle);	
}

void UCouchCharacterStateMachine::Tick(float DeltaTime)
{
	if(!CurrentState) return;
	CurrentState->StateTick(DeltaTime);
}

ACouchCharacter* UCouchCharacterStateMachine::GetCharacter() const
{
	return Character;
}

UCouchCharacterState* UCouchCharacterStateMachine::GetState(ECouchCharacterStateID StateID)
{
	for (UCouchCharacterState* State : AllStates)
	{
		if (StateID == State->GetStateID())
			return State;
	}
	return nullptr;
}

void UCouchCharacterStateMachine::ChangeState(ECouchCharacterStateID NextStateID)
{
	UCouchCharacterState* NextState = GetState(NextStateID);

	if (!NextState) return;

	if (CurrentState)
	{
		CurrentState->StateExit(NextStateID);
	}

	ECouchCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NextState;

	if (CurrentState)
	{
		CurrentState->StateEnter(PreviousStateID);
	}

	Character->PlayAnimMontage(CurrentState->AnimMontage);
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
