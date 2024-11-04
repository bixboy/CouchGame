
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CouchProjectile.generated.h"

UCLASS()
class COUCHGAME_API UCouchProjectile : public UActorComponent
{
	GENERATED_BODY()

public:
	UCouchProjectile();
	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void Initialize(const FVector& LaunchVelocity);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanMove = false;

private:
	UPROPERTY()
	FVector Velocity;      // Vélocité du projectile
	UPROPERTY()
	FVector Location;      // Position actuelle du projectile
	UPROPERTY()
	float TimeElapsed;     // Temps écoulé depuis le lancement
	
	const float Gravity = -980.0f;
};
