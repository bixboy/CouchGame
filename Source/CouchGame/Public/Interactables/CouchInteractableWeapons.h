// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/CouchMovement.h"
#include "Interactables/CouchInteractableMaster.h"
#include "Widget/CouchWidgetSpawn.h"
#include "CouchInteractableWeapons.generated.h"

UCLASS()
class COUCHGAME_API ACouchInteractableWeapons : public ACouchInteractableMaster
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="PlayFx"))
	void PlayFx();
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="PlayVibration"))
	void PlayVibration(UForceFeedbackEffect* Vibration);

public:
	ACouchInteractableWeapons();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(ACouchCharacter* Player) override;

	UFUNCTION()
	bool GetCanUse() const;
	UFUNCTION()
	void SetCanUse(bool Value);

	UFUNCTION()
	void DetachPlayer(ACouchCharacter* Player);
	
private:
	UFUNCTION()
	void Setup();
	
	UPROPERTY()
	bool CanUse = true;

#pragma region Setup
public:
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchMovement> MovementComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> PlayerPose;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchWidgetSpawn> WidgetComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxInteract;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> WidgetPose;
	
	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
#pragma endregion

#pragma region Movement
private:	
	UFUNCTION()
	virtual void StartMoveActor_Implementation(int InputDirection) override;
	UFUNCTION()
	virtual void StopMoveActor_Implementation() override;
	UPROPERTY(EditAnywhere)
	bool InvertInputMove = false;
	
#pragma endregion

#pragma region Widgets
public:
	UFUNCTION()
	void SetInteractWidget(TSubclassOf<ACouchWidget3D> InteractingWidget = nullptr);

	UPROPERTY(EditAnywhere, Category = DefaultValues)
	TSubclassOf<ACouchWidget3D> InteractWidget;
	
private:

	UPROPERTY()
	TSubclassOf<ACouchWidget3D> CurrentInteractWidget;

#pragma endregion

#pragma region Vibration

private:
	UPROPERTY(editAnywhere, Category = DefaultVibrationValue)
	UForceFeedbackEffect* MoveVibrationEffect;

#pragma endregion
};
