#pragma once

#include "CoreMinimal.h"
#include "CouchInteractableWeapons.h"
#include "Interfaces/CouchDamageable.h"
#include "Interfaces/CouchPickable.h"
#include "CouchUmbrella.generated.h"

UCLASS()
class COUCHGAME_API ACouchUmbrella : public ACouchInteractableWeapons, public ICouchDamageable, public ICouchPickable
{
	GENERATED_BODY()

public:
	ACouchUmbrella();
	virtual void Tick(float DeltaTime) override;

	virtual void PickUp_Implementation(ACouchCharacter* player) override;

	virtual void Drop_Implementation() override;

	virtual void InteractWithObject_Implementation(ACouchInteractableMaster* interactable) override;

	virtual bool IsPickable_Implementation() override;

	virtual void SetIsPickable_Implementation(bool isPickable) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* ShieldBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TObjectPtr<USoundBase> DamageSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	TObjectPtr<USoundBase> BrokeSound;
	void SpawnOrDeSpawnWarningWidget(bool Spawn);
	
private:
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TObjectPtr<USkeletalMesh> RepairingMesh;
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TObjectPtr<USkeletalMesh> DamagedMesh;
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TObjectPtr<USkeletalMesh> DestroyedMesh;
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TSubclassOf<ACouchWidget3D> WarningWidget;
	
#pragma region Life
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentPv() const;
	virtual ACouchPlank* Hit_Implementation(FHitResult, float RepairingTime = 0.f, float Scale = 0.f) override;
	
private:	
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	int MaxPv = 2;
	int CurrentPv = MaxPv;

	void DecreasePv();
	void HandleZeroPv();

#pragma endregion

#pragma region Repairing
public:
	virtual void Interact_Implementation(ACouchCharacter* Player) override;
	virtual float GetPercentRepair_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsInRepair() const;
	UFUNCTION(BlueprintCallable)
	void SetIsInRepair(bool Value);

private:
	UPROPERTY(EditAnywhere, Category = DefaultValues)
	float TimeToRepair;
	bool IsPlayerRepairing = false;
	float Timer;

	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TSubclassOf<ACouchWidget3D> RepairingWidget;

	void FinishRepairing();
	void StartRepair(ACouchCharacter* Player);
	void StopRepair();
	
#pragma endregion	
};
