// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchCannonBall.h"


// Sets default values
ACouchCannonBall::ACouchCannonBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACouchCannonBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACouchCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

