// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/CouchPlank.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Components/SceneComponent.h"
#include "Widget/CouchWidgetSpawn.h"
#include "Characters/CouchCharacter.h"


// Sets default values
ACouchPlank::ACouchPlank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BlockAll = CreateDefaultSubobject<UBoxComponent>("BlockAll");
	InterractiveBoxRange = CreateDefaultSubobject<UBoxComponent>("InterractiveBoxRange");
	HitMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlankMesh");
	WaterParticle = CreateDefaultSubobject<UNiagaraComponent>("Water");
	WidgetPos = CreateDefaultSubobject<USceneComponent>("WidgetPos");
	CouchWidgetSpawn = CreateDefaultSubobject<UCouchWidgetSpawn>("CouchWidgetSpawn");
	
	RootComponent = BlockAll;
	HitMesh->SetupAttachment(BlockAll);
	if (UStaticMesh* RandomStaticMesh = GetRandomStaticMesh()) HitMesh->SetStaticMesh(RandomStaticMesh); // Je mets un mesh aléatoire
	InterractiveBoxRange->SetupAttachment(HitMesh);
	WaterParticle->SetupAttachment(HitMesh);
	WidgetPos->SetupAttachment(HitMesh);


}

void ACouchPlank::Init(ABoatFloor* floor)
{
	
}

// Called when the game starts or when spawned
void ACouchPlank::BeginPlay()
{
	Super::BeginPlay();
}



UStaticMesh* ACouchPlank::GetRandomStaticMesh()
{
	if (DamagedPlanckMeshes.Num() == 0) return nullptr;
	int RandomIndex = FMath::RandRange(0, DamagedPlanckMeshes.Num() - 1);
	return DamagedPlanckMeshes[RandomIndex];
}

void ACouchPlank::Interact_Implementation(ACouchCharacter* Player)
{
	ICouchInteractable::Interact_Implementation(Player);
	
}




