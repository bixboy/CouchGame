// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CouchCharacterStateIdle.h"

#include "Characters/CouchCharacter.h"
#include "Characters/CouchCharacterSettings.h"
#include "Characters/CouchCharactersStateID.h"
#include "Characters/CouchCharacterStateMachine.h"

ECouchCharacterStateID UCouchCharacterStateIdle::GetStateID()
{
	return ECouchCharacterStateID::Idle;
}

void UCouchCharacterStateIdle::OnInputDash(FVector2D InputMove)
{

	StateMachine->ChangeState(ECouchCharacterStateID::Dash);
}

void UCouchCharacterStateIdle::StateEnter(ECouchCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::White,
		TEXT("Enter StateIdle")
	);

	Character->InputDashEvent.AddDynamic(this, &UCouchCharacterStateIdle::OnInputDash);
}

void UCouchCharacterStateIdle::StateExit(ECouchCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::White,
		TEXT("Exit StateIdle")
	);
	Character->InputDashEvent.RemoveDynamic(this, &UCouchCharacterStateIdle::OnInputDash);
}

void UCouchCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(
		-1,
		DeltaTime,
		FColor::White,
		TEXT("Tick StateIdle")
	);

	if (FMath::Abs(Character->GetInputMove().Size()) > CharacterSettings->InputMoveThreshold)
	{
		StateMachine->ChangeState(ECouchCharacterStateID::Walk);
	}
}
