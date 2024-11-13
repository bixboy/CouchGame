// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CouchCraftingTable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Curves/CurveFloat.h"
#include "Interactables/CouchPickableMaster.h"
#include "Crafting/FCraftingRecipe.h"
#include "Interactables/CouchPickableCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/CouchWidget3D.h"
#include "Widget/CouchWidgetSpawn.h"


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

	// Initialisation de PlateSuggestionPos
	PlateSuggestionPos = CreateDefaultSubobject<USceneComponent>(TEXT("PlateSuggestionPos"));
	PlateSuggestionPos->SetupAttachment(SkeletalMesh);

	// Initialisation de FinalDishSpawnPosition
	FinalDishSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FinalDishSpawnPosition"));
	FinalDishSpawnPosition->SetupAttachment(SkeletalMesh);

	FinalDishTargetPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FinalDishTargetPosition"));
	FinalDishTargetPosition->SetupAttachment(SkeletalMesh);

	WidgetSpawn = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("WidgetSpawn"));
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

const FCraftRecipe* ACouchCraftingTable::IsCraftingPossible()
{
	TArray<TSubclassOf<ACouchPickableMaster>> Ingredients;
	if (Plate1) Ingredients.Add(Plate1.GetClass());
	if (Plate2) Ingredients.Add(Plate2.GetClass());
	return IsCraftingPossible(Ingredients);
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

void ACouchCraftingTable::SpawnCraft()
{
	FVector StartLocation = FinalDishSpawnPosition->GetComponentLocation();
	FVector TargetLocation = FinalDishTargetPosition->GetComponentLocation();
	FVector SuggestedVelocity;
   
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
	 this,
	 SuggestedVelocity,
	 StartLocation,
	 TargetLocation,
	 0.f,
	 0.5f
  ); 
   
	FTransform Transform = FTransform(SuggestedVelocity.Rotation(), StartLocation);
	TObjectPtr<ACouchPickableCannonBall> CraftItem = GetWorld()->SpawnActor<ACouchPickableCannonBall>(ItemToCraft, Transform);
	ItemToCraft = nullptr;

	TArray<TObjectPtr<AActor>> ActorToIgnore;
	ActorToIgnore.Add(this);
	
	
	CraftItem->CouchProjectile->Initialize(SuggestedVelocity, ActorToIgnore);
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Spawn Craft Item");

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
	UpdateCraftSuggestion();
}

void ACouchCraftingTable::RemoveIngredient(ACouchPickableMaster* Ingredient)
{
	if (!Plate1 && !Plate2) return;
	if (Plate1.Get() != Ingredient && Plate2.Get() != Ingredient) return;
	if (Plate1.Get() == Ingredient)
	{
		Plate1 = nullptr;
	}
	else
	{
		Plate2 = nullptr;
	}
	UpdateCraftSuggestion();
}

bool ACouchCraftingTable::IsCraftingTableFull() const
{
	return Plate1 && Plate2;
}

void ACouchCraftingTable::CraftItem()
{
	if (!ItemToCraft) return;
	MoveTimeline.PlayFromStart();
}

// Called when the game starts or when spawned
void ACouchCraftingTable::BeginPlay()
{
	Super::BeginPlay();
	InitializeMoveTimeline();
}

void ACouchCraftingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveTimeline.TickTimeline(DeltaTime);
}

void ACouchCraftingTable::InitializeMoveTimeline()
{
	if (MoveCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("UpdateItemPosition"));
		MoveTimeline.AddInterpFloat(MoveCurve,ProgressFunction);
		MoveTimeline.SetLooping(false);
		
		FOnTimelineEvent OnTimelineFinishEvent;
		OnTimelineFinishEvent.BindUFunction(this, FName("OnMoveCompleted"));
		MoveTimeline.SetTimelineFinishedFunc(OnTimelineFinishEvent);
	}
}

void ACouchCraftingTable::UpdateItemPosition(float Alpha)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "TimelineTick");
	FVector TargetLocation = FinalDishSpawnPosition->GetComponentLocation();
	if (Plate1)
	{
		FVector StartLocation = Plate1->GetActorLocation();
		FVector StartScale = Plate1->GetActorScale();
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
		FVector NewScale = FMath::Lerp(StartScale, FVector::Zero(), Alpha);
		Plate1->SetActorLocation(NewLocation);
		Plate1->SetActorScale3D(NewScale);
	}
	if (Plate2)
	{
		FVector StartLocation = Plate2->GetActorLocation();
		FVector StartScale = Plate2->GetActorScale();
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
		FVector NewScale = FMath::Lerp(StartScale, FVector::Zero(), Alpha);
		Plate2->SetActorLocation(NewLocation);
		Plate2->SetActorScale3D(NewScale);
	}
}

void ACouchCraftingTable::OnMoveCompleted()
{
	if (Plate1)
	{
		Plate1->Destroy();
		Plate1 = nullptr;
	}
	if (Plate2)
	{
		Plate2->Destroy();
		Plate2 = nullptr;
	}
	SpawnCraft();
	UpdateCraftSuggestion();
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

void ACouchCraftingTable::UpdateCraftSuggestion()
{
	if (const FCraftRecipe* Recipe = IsCraftingPossible())
	{
		if (Recipe->ResultWidget)
		{
			WidgetSpawn->SpawnWidget(Recipe->ResultWidget, PlateSuggestionPos);
		}
		ItemToCraft = Recipe->ResultObject;
	}
	else
	{
		if (WidgetSpawn->GetCurrentWidget()) WidgetSpawn->DestroyWidget();
		ItemToCraft = nullptr;
	}
}

void ACouchCraftingTable::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
}


