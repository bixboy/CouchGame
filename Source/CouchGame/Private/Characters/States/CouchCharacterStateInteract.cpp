// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/CouchCharacterStateInteract.h"
#include "Characters/CouchCharacter.h"
#include "Characters/CouchCharactersStateID.h"
#include "Characters/CouchCharacterStateMachine.h"
#include "Interactables/CouchInteractableMaster.h"
#include "Interfaces/CouchInteractable.h"


ECouchCharacterStateID UCouchCharacterStateInteract::GetStateID()
{
	return ECouchCharacterStateID::InteractingObject;
}

void UCouchCharacterStateInteract::StateEnter(ECouchCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	//GEngine->AddOnScreenDebugMessage(
	//	-1,
	//	3.f,
	//	FColor::Green,
	//	TEXT("Enter StateInteract")
	//);
	
	ICouchInteractable::Execute_Interact(Character->InteractingActor, Character);
}

void UCouchCharacterStateInteract::StateExit(ECouchCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	//GEngine->AddOnScreenDebugMessage(
	//	-1,
	//	3.f,
	//	FColor::Green,
	//	TEXT("Exit StateInteract")
	//);
	if (Character->InteractingActor && Character->IsInteracting /*&& Character->IsInInteractingRange*/ ) ICouchInteractable::Execute_Interact(Character->InteractingActor, Character);
}

void UCouchCharacterStateInteract::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(
	//	-1,
	//	DeltaTime,
	//	FColor::Green,
	//	TEXT("Tick StateInteract")
	//);
	if (!Character->InteractingActor && !Character->IsInteracting && !Character->IsInInteractingRange)
	{
		StateMachine->ChangeState(ECouchCharacterStateID::Idle);
	}
	
	
}
