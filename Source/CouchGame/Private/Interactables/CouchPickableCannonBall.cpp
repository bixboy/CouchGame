// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchPickableCannonBall.h"

#include "Interactables/CouchCatapult.h"


ACouchPickableCannonBall::ACouchPickableCannonBall()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(RootComponent);

	Down = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	Down->SetupAttachment(RootComponent);
}

void ACouchPickableCannonBall::InteractWithObject_Implementation(ACouchInteractableMaster* interactable)
{
	Super::InteractWithObject_Implementation(interactable);
	if (interactable->IsA(ACouchCatapult::StaticClass()))
	{
		ACouchCatapult* catapult = Cast<ACouchCatapult>(interactable);
		if (!catapult) return;
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsInteractCatapultWithAmmo = true;
		catapult->Reload(this); /// Il va falloir appelez cette fonction quand le joueur a fini son anim sinon
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsInteractCatapultWithAmmo = false; //ça va direct cancel l'anim
	}
}


