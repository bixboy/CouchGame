
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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void Initialize(const FVector& LaunchVelocity);

	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	bool GetCanMove();
	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void SetCanMove(bool Value);

private:
	FVector Velocity;      // Vélocité du projectile
	FVector Location;      // Position actuelle du projectile
	float TimeElapsed;     // Temps écoulé depuis le lancement
	const float Gravity = -980.0f;
	
	UPROPERTY(EditAnywhere)
	bool CanMove = false;
};
