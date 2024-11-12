#pragma once

#include "CoreMinimal.h"
#include "Components/CouchChargePower.h"
#include "CableComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/CouchInteractable.h"
#include "CouchFishingRod.generated.h"

class ACouchWidget3D;
class ACouchCharacter;
class ACouchLure;
class ACouchPickableMaster;

UCLASS()
class COUCHGAME_API ACouchFishingRod : public AActor, public ICouchInteractable
{
	GENERATED_BODY()

public:
	ACouchFishingRod();
	void SetupFishingRod(TObjectPtr<ACouchCharacter> Player);
	
	virtual bool IsUsedByPlayer_Implementation() override;
	virtual void StartChargeActor_Implementation() override;
	virtual void StopChargeActor_Implementation() override;

	void DestroyLureAndCable();
	void DestroyFishingRod();
	
	TObjectPtr<ACouchCharacter> GetCharacter() const;

#pragma region Default Values
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchChargePower> ChargePower;
	
private:
	UPROPERTY()
	TObjectPtr<ACouchCharacter> CurrentPlayer;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "2"))
	int CurrentTeam = 1;
	
	bool IsInCharge = false;

	
#pragma endregion	

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

#pragma region QTE Rewind

public:
	UFUNCTION()
	void StartQte();

private:
	bool InQte = false;

	UPROPERTY(EditAnywhere, Category = DefaultRewindValue)
	float QtePercent = 0.1f;
	
	UPROPERTY(EditAnywhere, Category = DefaultRewindValue)
	TSubclassOf<ACouchWidget3D> WidgetQte;

	UFUNCTION()
	void RewindQte();
	
#pragma endregion	
	
};
