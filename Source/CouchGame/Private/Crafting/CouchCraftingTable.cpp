// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CouchCraftingTable.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "Interactables/CouchPickableMaster.h"
#include "Crafting/FCraftingRecipe.h"


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

	TableInteractiveZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TableInteractiveZone"));
	TableInteractiveZone->SetupAttachment(SkeletalMesh);
	
	// Initialisation de Table
	Table = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Table"));
	Table->SetupAttachment(TableInteractiveZone);

	// Initialisation de Plate1Position
	Plate1Position = CreateDefaultSubobject<USceneComponent>(TEXT("Plate1Position"));
	Plate1Position->SetupAttachment(Table);

	// Initialisation de Plate2Position
	Plate2Position = CreateDefaultSubobject<USceneComponent>(TEXT("Plate2Position"));
	Plate2Position->SetupAttachment(Table);

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

const FCraftRecipe* ACouchCraftingTable::IsCraftingPossible(
	const TArray<TSubclassOf<ACouchPickableMaster>>& Ingredients)
{
	for (const FCraftRecipe& Recipe : CraftRecipes)
	{
		if (AreArraysEqualIgnoringOrder(Recipe.Ingredients, Ingredients))
		{
			return &Recipe;
		}
	}
	return nullptr;
}

bool ACouchCraftingTable::AreArraysEqualIgnoringOrder(const TArray<TSubclassOf<ACouchPickableMaster>>& Array1,
	const TArray<TSubclassOf<ACouchPickableMaster>>& Array2) const
{
	if (Array1.Num() != Array2.Num())
	{
		return false;
	}
	
	TArray<TSubclassOf<ACouchPickableMaster>> SortedArray1 = Array1;
	TArray<TSubclassOf<ACouchPickableMaster>> SortedArray2 = Array2;

	SortedArray1.Sort();
	SortedArray2.Sort();

	// Compare les deux tableaux triés
	return SortedArray1 == SortedArray2;
}

void ACouchCraftingTable::AddIngredient(ACouchPickableMaster* Ingredient)
{
	if (Plate1 && Plate2) return;
	if (!Plate1)
	{
		Plate1 = Ingredient;
		if (Plate1Position)
		{
			PlaceActor(Plate1, Plate1Position);
		}
	}
	else
	{
		Plate2 = Ingredient;
		if (Plate2Position)
		{
			PlaceActor(Plate2, Plate2Position);
		}
	}
}

void ACouchCraftingTable::RemoveIngredient(ACouchPickableMaster* Ingredient)
{
	if (!Plate1 && !Plate2) return;
	if (Plate1.Get() != Ingredient && Plate2.Get() != Ingredient) return;
	if (Plate1.Get() == Ingredient)
	{
		Plate1 = nullptr;
	}
	else if (Plate2.Get() == Ingredient)
	{
		Plate2 = nullptr;
	}
}

bool ACouchCraftingTable::IsCraftingTableFull() const
{
	return Plate1 && Plate2;
}

void ACouchCraftingTable::PlaceActor(ACouchPickableMaster* Ingredient, USceneComponent* Position)
{
	Ingredient->SetActorLocation(Position->GetComponentLocation());
	Ingredient->SetActorRotation(Position->GetComponentRotation());
	FTransform Transform = FTransform(FRotator::ZeroRotator,
			Position->GetComponentLocation(),
			Ingredient->GetActorScale());
	Ingredient->SetActorTransform(Transform);
	Ingredient->AttachToComponent(Position, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ACouchCraftingTable::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
}


