// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CouchCharacterStateWalk.h"

#include "Characters/CouchCharactersStateID.h"

ECouchCharacterStateID UCouchCharacterStateWalk::GetStateID()
{
	return ECouchCharacterStateID::Walk;
}

void UCouchCharacterStateWalk::StateEnter(ECouchCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Blue,
		TEXT("Enter StateWalk")
	);
}

void UCouchCharacterStateWalk::StateExit(ECouchCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Blue,
		TEXT("Exit StateWalk")
	);
}
