// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mud.generated.h"

struct FItemPhysicsVelocity;
class UBoxComponent;
class ACouchCharacter;
class ACouchPickableMaster;

UCLASS()
class COUCHGAME_API AMud : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMud();

	void Init(float InPlayerSlowingSpeed, FVector2f InItemsSlowingSpeed);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	float PlayerSlowingSpeed;
	FVector2f ItemsSlowingSpeed;

	TArray<FItemPhysicsVelocity> PickableBaseVelocities;

	TMap<TObjectPtr<ACouchCharacter>, float> CharactersBaseSpeed;

	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

USTRUCT()
struct FItemPhysicsVelocity
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<ACouchPickableMaster> Pickable;

	float LinearDamping;

	float AngularDamping;
	
	bool operator==(const FItemPhysicsVelocity& Other) const;
};

