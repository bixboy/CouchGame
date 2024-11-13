// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchPickableCannonBall.h"

#include "Interactables/CouchCatapult.h"


ACouchPickableCannonBall::ACouchPickableCannonBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Base = Mesh;
	RootComponent = Base;

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
		catapult->Reload(this);
	}
}


