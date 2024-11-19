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
	virtual void Tick(float DeltaSeconds) override;
	void SetupFishingRod(TObjectPtr<ACouchCharacter> Player);
	
	virtual bool IsUsedByPlayer_Implementation() override;
	virtual void StartChargeActor_Implementation() override;
	virtual void StopChargeActor_Implementation() override;

	void DestroyLureAndCable();
	void DestroyFishingRod();

	TObjectPtr<ACouchCharacter> GetCharacter() const;
	int GetTeam() const;

#pragma region Default Values
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> WidgetPose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchChargePower> ChargePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchWidgetSpawn>WidgetSpawner;

	UPROPERTY()
	TObjectPtr<ACouchCharacter> CurrentPlayer;
	
private:
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TObjectPtr<UClass> PowerChargeWidget;
	
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "2"), Category = DefaultValue)
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
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float CableScale = 1.0f;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
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
	UPROPERTY(EditAnywhere, Category = DefaultRewindValue)
	float StopRewindZ = 2.f;

	void RewindCable(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void SpawnPickableObject();
	FVector GetRandomPos(float MinDistance, float MaxDistance, float Width);

public:
	void StopRewindCable();
	bool isPlayerFishing;

#pragma endregion

#pragma region QTE Rewind

public:
	UFUNCTION()
	void StartQte();
	UFUNCTION()
	void StopQte();

private:
	bool InQte = false;

	UPROPERTY(EditAnywhere, Category = DefaultRewindValue)
	float QtePercent = 0.1f;

	UFUNCTION()
	void RewindQte();
	
#pragma endregion	
	
};
