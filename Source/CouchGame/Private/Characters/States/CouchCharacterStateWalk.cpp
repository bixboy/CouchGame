// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CouchCharacterStateWalk.h"

#include "Characters/CouchCharacter.h"
#include "Characters/CouchCharactersStateID.h"
#include "Kismet/KismetMathLibrary.h"

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

void UCouchCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(
		-1,
		DeltaTime,
		FColor::Blue,
		TEXT("Tick StateWalk")
	);
	if (Character)
	{
		Character->AddMovementInput
		(UKismetMathLibrary::GetRightVector(Character->GetMesh()->GetRelativeRotation()), MoveSpeedMax);
	}
}
