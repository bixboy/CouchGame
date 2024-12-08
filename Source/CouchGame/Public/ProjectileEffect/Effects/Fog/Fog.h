#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Fog.generated.h"

UCLASS()
class COUCHGAME_API AFog : public AActor
{
	GENERATED_BODY()

public:
	AFog();
	void UpdateFog(float Value, float MaxOpacity) const;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
};
