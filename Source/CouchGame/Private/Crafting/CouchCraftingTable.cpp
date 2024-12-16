// Fill out your copyright notice in the Description page of Project Settings.


#include "Crafting/CouchCraftingTable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Crafting/CouchOctopusAnimationManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Curves/CurveFloat.h"
#include "Interactables/CouchPickableMaster.h"
#include "Crafting/FCraftingRecipe.h"
#include "Interactables/CouchPickableCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/CouchCraftValidateWidget.h"
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
	TableInteractiveZone->SetupAttachment(RootComponent);
	
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
	PlateSuggestionPos->SetupAttachment(RootComponent);

	// Initialisation de FinalDishSpawnPosition
	FinalDishSpawnPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FinalDishSpawnPosition"));
	FinalDishSpawnPosition->SetupAttachment(RootComponent);

	FinalDishTargetPosition = CreateDefaultSubobject<USceneComponent>(TEXT("FinalDishTargetPosition"));
	FinalDishTargetPosition->SetupAttachment(RootComponent);

	WidgetSpawn = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("WidgetSpawn"));
}

const FCraftRecipe* ACouchCraftingTable::IsCraftingPossible(
	const TArray<TSubclassOf<ACouchPickableMaster>>& Ingredients)
{
	if (Ingredients.IsEmpty()) return nullptr;
	for (const FCraftRecipe& Recipe : CraftRecipes)
	{
		if (AreArraysEqualIgnoringOrder(Recipe.Ingredients, Ingredients) && Recipe.ResultObject)
		{
			return &Recipe;
		}
	}
	return nullptr;
}

// Called when the game starts or when spawned
void ACouchCraftingTable::BeginPlay()
{
	Super::BeginPlay();
	InitializeMoveTimeline();
	AnimationManager = NewObject<UCouchOctopusAnimationManager>(this);

	if (ValidateWidget)
	{
		FVector LocationTemp = Plate1Position->GetComponentLocation();
		FTransform PlateTransform = FTransform(Plate1Position->GetComponentRotation(), LocationTemp + OffsetWiget);
		ValidateWidgetLeft = GetWorld()->SpawnActor<ACouchCraftValidateWidget>(ValidateWidget, PlateTransform);
		ValidateWidgetLeft->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		LocationTemp = Plate2Position->GetComponentLocation();
		PlateTransform = FTransform(Plate2Position->GetComponentRotation(),LocationTemp + OffsetWiget);
		ValidateWidgetRight = GetWorld()->SpawnActor<ACouchCraftValidateWidget>(ValidateWidget, PlateTransform);
		ValidateWidgetRight->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ACouchCraftingTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MoveTimeline.IsPlaying()) MoveTimeline.TickTimeline(DeltaTime);
}

const FCraftRecipe* ACouchCraftingTable::IsCraftingPossible()
{
	TArray<TSubclassOf<ACouchPickableMaster>> Ingredients;
	if (Plate1) Ingredients.Add(Plate1.GetClass());
	if (Plate2) Ingredients.Add(Plate2.GetClass());
	return IsCraftingPossible(Ingredients);
}

int ACouchCraftingTable::GetTeam()
{
	return CurrentTeam;
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
	// ICI LE CRASH DU LA PREZ
	if (TObjectPtr<ACouchPickableCannonBall> CraftItem = GetWorld()->SpawnActor<ACouchPickableCannonBall>(ItemToCraft, Transform))
	{
		ItemToCraft = nullptr;

		TArray<TObjectPtr<AActor>> ActorToIgnore;
		ActorToIgnore.Add(this);
	
	
		CraftItem->CouchProjectile->Initialize(SuggestedVelocity, ActorToIgnore);
		AnimationManager->IsCooking = false;
		AnimationManager->HasObjectOnTheTable = false;	
	}
	else
	{
		if (!ItemToCraft) GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "ItemToCraft");
		if (!Transform.IsValid()) GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Red, "Transform not valid");
	}
}

bool ACouchCraftingTable::IsAutoCookPossible()
{
	if (!AutoCookWhen2IngredientsSet) return false;
	if (Plate1 && Plate2) return true;
	return false;
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
			PlayFX(ImpactObject, Plate1Position->GetComponentLocation());
			if(ValidateWidgetLeft)
			{
				ValidateWidgetLeft->SetIsValid(true);
				ValidateWidgetLeft->SetActorHiddenInGame(false);
			}
			UGameplayStatics::PlaySound2D(this, SpawnIngredientSound);
		}
	}
	else
	{
		Plate2 = Ingredient;
		if (Plate2Position)
		{
			PlaceActor(Plate2, Plate2Position);
			PlayFX(ImpactObject, Plate2Position->GetComponentLocation());
			if(ValidateWidgetRight)
			{
				ValidateWidgetRight->SetIsValid(true);
				ValidateWidgetRight->SetActorHiddenInGame(false);
			}
			UGameplayStatics::PlaySound2D(this, SpawnIngredientSound);
		}
	}
	AnimationManager->HasObjectOnTheTable = true;
	UpdateCraftSuggestion();
}

void ACouchCraftingTable::RemoveIngredient(ACouchPickableMaster* Ingredient)
{
	if (!Plate1 && !Plate2) return;
	if (Plate1.Get() != Ingredient && Plate2.Get() != Ingredient) return;
	if (Plate1.Get() == Ingredient)
	{
		Plate1 = nullptr;
		if(ValidateWidgetLeft)ValidateWidgetLeft->SetIsValid(false);
	}
	else
	{
		Plate2 = nullptr;
		if(ValidateWidgetRight)ValidateWidgetRight->SetIsValid(false);
	}
	
	if (!Plate1 && !Plate2)
	{
		AnimationManager->HasObjectOnTheTable = false;
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
	if (Plate1) ICouchPickable::Execute_SetIsPickable(Plate1, false);
	if (Plate2) ICouchPickable::Execute_SetIsPickable(Plate2, false);
	if (!MoveTimeline.IsPlaying())
	{
		MoveTimeline.PlayFromStart();
		AnimationManager->IsCooking = true;
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsCheckingChef = false;
		if(ValidateWidgetLeft && ValidateWidgetRight)
		{
			ValidateWidgetLeft->SetIsValid(false);
			ValidateWidgetLeft->SetActorHiddenInGame(true);
			ValidateWidgetRight->SetIsValid(false);
			ValidateWidgetRight->SetActorHiddenInGame(true);
		}
	} 
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
	// GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "TimelineTick");
	if (IsUpdating) return;
	IsUpdating = true;
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
	IsUpdating = false;
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

void ACouchCraftingTable::HiddWidget(bool Val)
{
	ValidateWidgetRight->SetActorHiddenInGame(Val);
	ValidateWidgetLeft->SetActorHiddenInGame(Val);
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
		if (IsAutoCookPossible())
		{
			ItemToCraft = Recipe->ResultObject;
			CraftItem();
			return;
		}
		
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
	// Super::Interact_Implementation(Player);
}


