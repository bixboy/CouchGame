// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchPickableCannonBall.h"


ACouchPickableCannonBall::ACouchPickableCannonBall()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(RootComponent);

	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(RootComponent);

	Down = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	Down->SetupAttachment(RootComponent);
}


