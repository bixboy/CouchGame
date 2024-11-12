// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CouchCraftingTable.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
ACouchCraftingTable::ACouchCraftingTable()
{
	// Activer le Tick si nécessaire
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// Initialisation de SkeletalMesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	// Initialisation de Arrow
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

	// Initialisation de Table
	Table = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table"));
	Table->SetupAttachment(SkeletalMesh);

	// Initialisation de Plate1Position
	Plate1Position = CreateDefaultSubobject<USceneComponent>(TEXT("Plate1Position"));
	Plate1Position->SetupAttachment(Table);

	// Initialisation de Plate1Box
	Plate1Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Plate1Box"));
	Plate1Box->SetupAttachment(Plate1Position);

	// Initialisation de Plate2Position
	Plate2Position = CreateDefaultSubobject<USceneComponent>(TEXT("Plate2Position"));
	Plate2Position->SetupAttachment(Table);

	// Initialisation de Plate2Box
	Plate2Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Plate2Box"));
	Plate2Box->SetupAttachment(Plate2Position);

	// Initialisation de FillBarPosition
	FillBarPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FillBarPosition"));
	FillBarPosition->SetupAttachment(SkeletalMesh);

	// Initialisation de PlateSuggestionPos
	PlateSuggestionPos = CreateDefaultSubobject<USceneComponent>(TEXT("PlateSuggestionPos"));
	PlateSuggestionPos->SetupAttachment(SkeletalMesh);

	// Initialisation de FinalDishSpawnPosition
	FinalDishSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FinalDishSpawnPosition"));
	FinalDishSpawnPosition->SetupAttachment(SkeletalMesh);
}

// Called when the game starts or when spawned
void ACouchCraftingTable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACouchCraftingTable::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
}


