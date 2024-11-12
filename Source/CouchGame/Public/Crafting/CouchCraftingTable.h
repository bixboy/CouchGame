#pragma once

#include "CoreMinimal.h"
#include "Interactables/CouchInteractableMaster.h"
#include "CouchCraftingTable.generated.h"


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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

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
	TObjectPtr<USceneComponent> FillBarPosition;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> PlateSuggestionPos;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> FinalDishSpawnPosition;
#pragma endregion

#pragma region Crafting
public:
	const FCraftRecipe* IsCraftingPossible(const TArray<TSubclassOf<ACouchPickableMaster>>& Ingredients);
private:
	UPROPERTY(EditAnywhere, Category = Crafting)
	TArray<FCraftRecipe> CraftRecipes;

	bool AreArraysEqualIgnoringOrder(const TArray<TSubclassOf<ACouchPickableMaster>>& Array1,
	const TArray<TSubclassOf<ACouchPickableMaster>>& Array2) const;
	void PlaceActor(ACouchPickableMaster* Ingredient, USceneComponent* Position);

public:
	void AddIngredient(ACouchPickableMaster* Ingredient);

	void RemoveIngredient(ACouchPickableMaster* Ingredient);

	bool IsCraftingTableFull() const;
#pragma endregion
};


