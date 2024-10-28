// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchLure.h"

#include "Components/CouchProjectile.h"

ACouchLure::ACouchLure()
{
	PrimaryActorTick.bCanEverTick = true;
	CouchProjectile = CreateDefaultSubobject<UCouchProjectile>(TEXT("ProjectileComponent"));
}

void ACouchLure::BeginPlay()
{
	Super::BeginPlay();
}

void ACouchLure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

