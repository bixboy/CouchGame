// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/CouchUmbrella.h"


ACouchUmbrella::ACouchUmbrella()
{
	PrimaryActorTick.bCanEverTick = true;
	ShieldBox = CreateDefaultSubobject<UBoxComponent>("ShieldBox");
	ShieldBox->SetupAttachment(RootComponent);
}

