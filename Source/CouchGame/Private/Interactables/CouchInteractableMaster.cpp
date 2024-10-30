// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/CouchInteractableMaster.h"


ACouchInteractableMaster::ACouchInteractableMaster()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ACouchInteractableMaster::IsUsedByPlayer_Implementation()
{
	ICouchInteractable::IsUsedByPlayer_Implementation();
	return PlayerIsIn;
}

void ACouchInteractableMaster::Interact_Implementation(ACouchCharacter* Player)
{
	ICouchInteractable::Interact_Implementation(Player);
	if (!Execute_IsUsedByPlayer(this))
	{
		CurrentPlayer = Player;
		PlayerIsIn = true;
	}
}

AActor* ACouchInteractableMaster::GetCurrentPlayer() const {return CurrentPlayer;}

void ACouchInteractableMaster::SetCurrentPlayer(ACouchCharacter* Player) {CurrentPlayer = Player;}

void ACouchInteractableMaster::SetPlayerIsIn(bool IsIn) {PlayerIsIn = IsIn;}

