// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactables/CouchPickableCannonBall.h"
#include "CouchCannonBall.generated.h"

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

	void InitCanonBall(TObjectPtr<ACouchStaticCanonBall> PickableCannonBall);

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
