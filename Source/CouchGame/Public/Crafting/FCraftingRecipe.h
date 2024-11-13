// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FCraftingRecipe.generated.h"

class ACouchPickableCannonBall;
class ACouchPickableMaster;

USTRUCT()
struct FCraftRecipe
{
	GENERATED_BODY()

	// Les ingrédients nécessaires au craft
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ACouchPickableMaster>> Ingredients;

	// Le résultat unique du craft (classe de l'objet produit)
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACouchPickableCannonBall> ResultObject;

	// Le widget associé au résultat du craft
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACouchWidget3D> ResultWidget;
};

