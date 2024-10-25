// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchSplineLinePath.h"

#include "Components/SplineComponent.h"


// Sets default values
ACouchSplineLinePath::ACouchSplineLinePath()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACouchSplineLinePath::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACouchSplineLinePath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

