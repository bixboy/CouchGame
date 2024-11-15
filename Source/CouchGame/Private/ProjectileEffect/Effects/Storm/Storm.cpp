// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Storm/Storm.h"


// Sets default values
AStorm::AStorm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
}


