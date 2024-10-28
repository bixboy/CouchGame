// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CouchCharacterStateWalk.h"

#include "Characters/CouchCharacter.h"
#include "Characters/CouchCharacterSettings.h"
#include "Characters/CouchCharactersStateID.h"
#include "Characters/CouchCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ECouchCharacterStateID UCouchCharacterStateWalk::GetStateID()
{
	return ECouchCharacterStateID::Walk;
}

void UCouchCharacterStateWalk::OnInputDash(FVector2D InputMove)
{
	if (Character->GetCanDash())
	{
		StateMachine->ChangeState(ECouchCharacterStateID::Dash);
	}
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
	Character->GetCharacterMovement()->MaxWalkSpeed = MoveSpeedMax;
	Character->InputDashEvent.AddDynamic(this, &UCouchCharacterStateWalk::OnInputDash);
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
	Character->InputDashEvent.RemoveDynamic(this, &UCouchCharacterStateWalk::OnInputDash);
}

void UCouchCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(
		-1,
		DeltaTime,
		FColor::Blue,
		TEXT("Tick StateWalk")
	);
	if (FMath::Abs(Character->GetInputMove().Size()) < CharacterSettings->InputMoveThreshold)
	{
		StateMachine->ChangeState(ECouchCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrient(Character->GetOrient());
		Character->MoveInDirectionOfRotation(MoveSpeedMax);
	}
}
