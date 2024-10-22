// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CouchCharacterState.h"

#include "Characters/CouchCharactersStateID.h"
#include "Characters/CouchCharacterStateMachine.h"


// Sets default values for this component's properties
UCouchCharacterState::UCouchCharacterState()
{
	PrimaryComponentTick.bCanEverTick = true;
}

ECouchCharacterStateID UCouchCharacterState::GetStateID()
{
	return ECouchCharacterStateID::None;
}

void UCouchCharacterState::StateInit(UCouchCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Magenta,
		FString::Printf(TEXT("Init State %d"), GetStateID())
	);
	
}

void UCouchCharacterState::StateEnter(ECouchCharacterStateID PreviousStateID)
{
}

void UCouchCharacterState::StateExit(ECouchCharacterStateID NextStateID)
{
}


