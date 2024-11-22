#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CouchOctopusAnimationManager.generated.h"

UCLASS(Blueprintable, BlueprintType)
class COUCHGAME_API UCouchOctopusAnimationManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool HasObjectOnTheTable = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "States")
	bool IsCooking = false;
};
