// Fill out your copyright notice in the Description page of Project Settings.

#include "CouchFishingObject.h"

#include "Components/BoxComponent.h"

ACouchFishingObject::ACouchFishingObject()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;

	Collider = CreateDefaultSubobject<USphereComponent>("Collider");
	Collider->SetupAttachment(Mesh);
}


