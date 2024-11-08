
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "CouchProjectile.generated.h"

UCLASS()
class COUCHGAME_API UCouchProjectile : public UActorComponent
{
	GENERATED_BODY()

public:
	UCouchProjectile();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> ProjectileCollision;

	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void Initialize(const FVector& LaunchVelocity, const TArray<AActor*> ActorsToIgnore);

	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	bool GetCanMove();
	UFUNCTION(BlueprintCallable, Category = "Couch Projectile")
	void SetCanMove(bool Value);

private:
	FVector Velocity;      // Vélocité du projectile
	FVector Location;      // Position actuelle du projectile
	float TimeElapsed;     // Temps écoulé depuis le lancement
	const float Gravity = -980.0f;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> IgnoredActors;
	
	UPROPERTY(EditAnywhere)
	bool CanMove = false;
};
