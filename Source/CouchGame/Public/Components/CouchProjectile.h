
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "CouchProjectile.generated.h"

class AShadowProjectile;

UCLASS()
class COUCHGAME_API UCouchProjectile : public UActorComponent
{
	GENERATED_BODY()

public:
	UCouchProjectile();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USphereComponent> ProjectileCollision;

	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void Initialize(const FVector& LaunchVelocity, const TArray<AActor*> ActorsToIgnore, bool UnableCollision = true);

	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	bool GetCanMove() const;
	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void SetCanMove(bool Value);

private:
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float SpeedMultiplier = 1.f;
	FVector Velocity;      // Vélocité du projectile
	FVector Location;      // Position actuelle du projectile
	float TimeElapsed;     // Temps écoulé depuis le lancement
	const float Gravity = -980.0f;
	
	bool CollisionIsActive;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoredActors;
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	bool CanMove = false;

	float ReactivatePhysicsCountdown = -1.f;
	float ReactivatePhysicsDelay = 0.05f;
};
