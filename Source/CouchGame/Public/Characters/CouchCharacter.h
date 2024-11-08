// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "CouchCharacter.generated.h"

class ACouchFishingRod;
class ACouchInteractableMaster;
class USphereComponent;
class UCouchCharacterInputData;
class UCouchCharacterStateMachine;
class UInputMappingContext;
class UEnhancedInputComponent;
class UCouchCharacterSettings;

UCLASS()
class COUCHGAME_API ACouchCharacter : public ACharacter
{
	GENERATED_BODY()
#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ACouchCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion
#pragma region Move And Orient
public:
	FVector2D GetOrient() const;

	void SetOrient(FVector2D NewOrient);

	void MoveInDirectionOfRotation(float InputStrength);

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector2D Orient = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere)
	float CharacterRotationSpeed = 20.0f;
	void RotateMeshUsingOrient(float DeltaTime) const;

#pragma endregion
#pragma region State Machine
public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCouchCharacterStateMachine> StateMachine;
#pragma endregion
#pragma region InputData / MappingContext
	public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<UCouchCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;

#pragma endregion
#pragma region InputMove
	
public:
	FVector2D GetInputMove() const;

protected:
	UPROPERTY()
	FVector2D InputMove = FVector2D::ZeroVector;

private:
	void BindInputMoveAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMove(const FInputActionValue& InputActionValue);

	UPROPERTY()
	const UCouchCharacterSettings* CharacterSettings;

#pragma endregion
#pragma region Dash
	
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputDashEvent, FVector2D, InputMove);
	public:
	bool GetCanDash() const;

	UPROPERTY()
	FInputDashEvent InputDashEvent;

	UPROPERTY()
	float DashDuration;


private:
	UPROPERTY(EditAnywhere)
	bool CanDash = true;

	UPROPERTY()
	bool CanDashAgain = true;

	UPROPERTY()
	float DashTimer;
	
	
	void OnInputDash(const FInputActionValue& InputActionValue);
#pragma endregion
#pragma region Interraction
public:
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractionZone;
	UFUNCTION()
	void OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep,const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY()
	TArray<TObjectPtr<ACouchInteractableMaster>> InteractingActors;
	UPROPERTY()
	TObjectPtr<ACouchInteractableMaster> InteractingActor;
	TObjectPtr<ACouchInteractableMaster> FindNearestInteractingActor() const;
	bool IsInInteractingRange;
	bool IsInteracting;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* PickUpItemPosition;
private :
	void BindInputInteractAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputInteract(const FInputActionValue& InputActionValue);

	void OnInputFire(const FInputActionValue& InputActionValue);
	float InputFireValue = 0.f;

	void OnInputMoveInteracting(const FInputActionValue& InputActionValue);

	void OnInputFishing(const FInputActionValue& InputActionValue);

	UPROPERTY()
	TObjectPtr<ACouchFishingRod> FishingRod;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TSubclassOf<ACouchFishingRod> FishingRodSpawn;
#pragma endregion
#pragma region Hold Item

public:
	bool GetIsHoldingItem() const;
private:
	bool IsHoldingItem;
#pragma endregion
};
