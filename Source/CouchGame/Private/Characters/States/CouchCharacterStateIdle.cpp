// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CouchCharacterStateIdle.h"

#include "Characters/CouchCharacter.h"
#include "Characters/CouchCharactersStateID.h"
#include "Characters/CouchCharacterStateMachine.h"

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

void UCouchCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(
		-1,
		DeltaTime,
		FColor::Black,
		TEXT("Tick StateIdle")
	);

	if (FMath::Abs(Character->GetInputMove().Size()) > 0.1f)
	{
		StateMachine->ChangeState(ECouchCharacterStateID::Walk);
	}
}
