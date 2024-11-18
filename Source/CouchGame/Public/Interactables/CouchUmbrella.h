// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CouchInteractableWeapons.h"
#include "Interfaces/CouchDamageable.h"
#include "CouchUmbrella.generated.h"

UCLASS()
class COUCHGAME_API ACouchUmbrella : public ACouchInteractableWeapons, public ICouchDamageable
{
	GENERATED_BODY()

public:
	ACouchUmbrella();
	void Tick(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* ShieldBox;
	
private:
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TObjectPtr<USkeletalMesh> RepairingMesh;
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TObjectPtr<USkeletalMesh> DamagedMesh;
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TObjectPtr<USkeletalMesh> DestroyedMesh;
	
#pragma region Life
public:
	UFUNCTION()
	int GetCurrentPv() const;
	virtual void Hit_Implementation(FHitResult, float RepairingTime = 0.f, float Scale = 0.f) override;
	
private:	
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	int MaxPv = 2;
	int CurrentPv = MaxPv;

	void DecreasePv();

#pragma endregion

#pragma region Repairing
public:
	virtual void Interact_Implementation(ACouchCharacter* Player) override;
	virtual float GetPercentRepair_Implementation() override;

private:
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	float TimeToRepair;
	bool IsPlayerRepairing = false;
	float Timer;

	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TSubclassOf<ACouchWidget3D> RepairingWidget;

	void FinishRepairing();
	
#pragma endregion	
};
