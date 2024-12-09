#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShadowProjectile.generated.h"

UCLASS()
class COUCHGAME_API AShadowProjectile : public AActor
{
	GENERATED_BODY()

public:
	AShadowProjectile();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
