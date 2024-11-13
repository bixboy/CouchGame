#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Interactables/CouchInteractableMaster.h"
#include "CouchCraftingTable.generated.h"


struct FTimeline;
class UTimelineComponent;
class ACouchPickableCannonBall;
class ACouchWidget3D;
class UCouchWidgetSpawn;
struct FCraftRecipe;
class ACouchPickableMaster;

UCLASS()
class COUCHGAME_API ACouchCraftingTable : public ACouchInteractableMaster
{
	GENERATED_BODY()

#pragma region Unreal Default
public:
	// Sets default values for this actor's properties
	ACouchCraftingTable();
	virtual void Interact_Implementation(ACouchCharacter* Player) override;


private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Table;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> TableInteractiveZone;

	UPROPERTY()
	TObjectPtr<ACouchPickableMaster> Plate1;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Plate1Position;
	
	UPROPERTY()
	TObjectPtr<ACouchPickableMaster> Plate2;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Plate2Position;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> PlateSuggestionPos;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> FinalDishSpawnPosition;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> FinalDishTargetPosition;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCouchWidgetSpawn> WidgetSpawn;
#pragma endregion

#pragma region Crafting
public:
	const FCraftRecipe* IsCraftingPossible(const TArray<TSubclassOf<ACouchPickableMaster>>& Ingredients);
	const FCraftRecipe* IsCraftingPossible();
private:
	UPROPERTY(EditAnywhere, Category = Crafting)
	TArray<FCraftRecipe> CraftRecipes;

	bool AreArraysEqualIgnoringOrder(const TArray<TSubclassOf<ACouchPickableMaster>>& Array1,
	const TArray<TSubclassOf<ACouchPickableMaster>>& Array2) const;
	void PlaceActor(ACouchPickableMaster* Ingredient, USceneComponent* Position);

	void UpdateCraftSuggestion();

	TSubclassOf<ACouchPickableCannonBall> ItemToCraft;

	void SpawnCraft();

public:
	void AddIngredient(ACouchPickableMaster* Ingredient);

	void RemoveIngredient(ACouchPickableMaster* Ingredient);

	bool IsCraftingTableFull() const;

	void CraftItem();

#pragma endregion

#pragma region Craft Animation

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	FTimeline MoveTimeline;

	UPROPERTY(EditAnywhere, Category = Timeline)
	UCurveFloat* MoveCurve;
	
	void InitializeMoveTimeline();

	UFUNCTION()
	void UpdateItemPosition(float Alpha);

	UFUNCTION()
	void OnMoveCompleted();
#pragma endregion
};


