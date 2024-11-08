#pragma once

#include "CoreMinimal.h"
#include "Components/CouchChargePower.h"
#include "CableComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "CouchFishingRod.generated.h"

class ACouchCharacter;
class ACouchLure;
class ACouchPickableMaster;

UCLASS()
class COUCHGAME_API ACouchFishingRod : public AActor, public ICouchInteractable
{
	GENERATED_BODY()

public:
	ACouchFishingRod();
	
	UFUNCTION()
	virtual bool IsUsedByPlayer_Implementation() override;
	UFUNCTION()
	virtual void StartChargeActor_Implementation() override;
	UFUNCTION()
	virtual void StopChargeActor_Implementation() override;

	UFUNCTION()
	void DestroyLureAndCable();

	UFUNCTION()
	void SetupFishingRod(ACouchCharacter* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchChargePower> ChargePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	TSubclassOf<ACouchPickableMaster> PickableObject;

private:
	UPROPERTY()
	TObjectPtr<ACouchCharacter> CurrentPlayer;

#pragma region Lure Actor
private:
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TObjectPtr<UClass> Lure;
	UPROPERTY()
	TObjectPtr<ACouchLure> LureRef;
	
	UFUNCTION()
	void SpawnLure();	
	
#pragma endregion	
	
#pragma region Cable Components
private:
	UPROPERTY(EditAnywhere, Category = DefaultCableValue)
	TObjectPtr<UCableComponent> Cable;
	UPROPERTY()
	float CableScale = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = DefaultCableValue)
	float MaxCableLength;
	UPROPERTY(EditAnywhere, Category = DefaultCableValue)
	float MinCableLength;
	
	UPROPERTY(EditAnywhere, Category = DefaultCableValue)
	TObjectPtr<UMaterial> CableMaterial;

	UFUNCTION()
	void InitializeCableAndConstraint();

#pragma endregion

#pragma region Rewind	
private:
	UPROPERTY(EditAnywhere, Category = DefaultRewindValue)
	float Threshold;
	UPROPERTY(EditAnywhere, Category = DefaultRewindValue)
	float RewindSpeed;
	UPROPERTY(EditAnywhere, Category = DefaultRewindValue)
	float StopRewindDistance = 100.f;
	
	float PreviousAngle;

	UFUNCTION(BlueprintCallable)
	void RewindCable(float DeltaTime, float JoystickX, float JoystickY);
	UFUNCTION(blueprintCallable)
	void StopRewindCable();
	UFUNCTION(BlueprintCallable)
	void SpawnPickableObject();
	
	FVector GetRandomPos(float MinDistance, float MaxDistance, float Width);

#pragma endregion	
	
};
