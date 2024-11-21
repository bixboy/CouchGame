// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/CouchPickableCannonBall.h"
#include "CouchCannonBall.generated.h"

enum class ECouchProjectileExecuteTime : uint8;
class ACouchProjectileEffect;
class ACouchStaticCanonBall;
class USphereComponent;

UCLASS()
class COUCHGAME_API ACouchCannonBall : public AActor
{
	GENERATED_BODY()

public:
	ACouchCannonBall();

	virtual void Tick(float DeltaTime) override;
	void Initialize(const FVector& LaunchVelocity);

	void InitCanonBall(TObjectPtr<ACouchStaticCanonBall> StaticCannonBall);

	UFUNCTION()
	void OnCannonBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FVector Velocity;      // Vélocité du projectile
	UPROPERTY()
	FVector Location;      // Position actuelle du projectile
	UPROPERTY()
	float TimeElapsed;     // Temps écoulé depuis le lancement
	const float Gravity = -980.0f;
	UPROPERTY()
	TArray<TObjectPtr<ACouchProjectileEffect>> ProjectileEffects;
	TArray<TSubclassOf<ACouchProjectileEffect>> ProjectileEffectsClass;
	bool HasEffectWithExecuteTime(ECouchProjectileExecuteTime ExecuteTime);
	void ExecuteEffectWithExecuteTime(ECouchProjectileExecuteTime ExecuteTime);
	void InitEffect(ACouchCannonBall* CanonBall = nullptr,
	                ACouchPickableCannonBall* PickCannonBall = nullptr, AActor* ActorToInteractWith = nullptr,
	                FHitResult HitResult = FHitResult());
	
	void InitEffectWithExecuteTime(ECouchProjectileExecuteTime ExecuteTime, ACouchCannonBall* CanonBall = nullptr,
		ACouchPickableCannonBall* PickCannonBall = nullptr, AActor* ActorToInteractWith = nullptr, FHitResult HitResult = FHitResult());
	
	float DelayBeforeEffect;
	float Timer;
	UPROPERTY()
	TObjectPtr<ACouchPickableCannonBall> PickableCannonBall;
	bool HasBeenLaunched;

#pragma region Mesh
private:
	UPROPERTY()
	UStaticMesh* Mesh;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Top;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Down;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	USphereComponent* Sphere;

#pragma endregion	
	
};
