#pragma once


#include "CoreMinimal.h"
#include "Components/CouchChargePower.h"
#include "CableComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CouchFishingRod.generated.h"


class ACouchLure;


UCLASS()
class COUCHGAME_API ACouchFishingRod : public AActor, public ICouchInteractable
{
	GENERATED_BODY()


public:
	ACouchFishingRod();
	virtual bool IsUsedByPlayer_Implementation() override;

public:
	virtual void StartChargeActor_Implementation() override;
	virtual void StopChargeActor_Implementation() override;
	
	UFUNCTION()
	void BeakCableConstraint() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchChargePower> ChargePower;


private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCableComponent> Cable;
	UPROPERTY()
	float CableScale = 1.0f;
	UPROPERTY()
	TObjectPtr<UMaterial> CableMaterial;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UClass> Lure;
	UPROPERTY()
	TObjectPtr<ACouchLure> LureRef;


	UPROPERTY(EditAnywhere)
	float Threshold;
	UPROPERTY(EditAnywhere)
	float RewindSpeed;


	UPROPERTY(EditAnywhere)
	float MaxCableLength;
	UPROPERTY(EditAnywhere)
	float MinCableLength;
  
	UFUNCTION()
	void SpawnLure();
	UFUNCTION()
	void InitializeCableAndConstraint();
	UFUNCTION(BlueprintCallable)
	void RewindCable(float DeltaTime, float JoystickX, float JoystickY);

	float PreviousAngle;
};
