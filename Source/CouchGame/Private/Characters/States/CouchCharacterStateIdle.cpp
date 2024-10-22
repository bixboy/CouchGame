// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CouchCharacterStateIdle.h"

#include "Characters/CouchCharactersStateID.h"

ECouchCharacterStateID UCouchCharacterStateIdle::GetStateID()
{
	return ECouchCharacterStateID::Idle;
}

void UCouchCharacterStateIdle::StateEnter(ECouchCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Black,
		TEXT("Enter StateIdle")
	);
}

void UCouchCharacterStateIdle::StateExit(ECouchCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Black,
		TEXT("Exit StateIdle")
	);
}
