#pragma once

#include "CoreMinimal.h"
#include "CouchCharacterAnimationManager.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Interfaces/CouchDamageable.h"
#include "CouchCharacter.generated.h"

class ACouchWidget3D;
class UCouchWidgetSpawn;
class UCouchWidgetPause;
class UCouchWidgetWin;
class UBoxComponent;
class AItemSpawnerManager;
class ACouchFishingRod;
class ACouchInteractableMaster;
class USphereComponent;
class UCouchCharacterInputData;
class UCouchCharacterStateMachine;
class UInputMappingContext;
class UEnhancedInputComponent;
class UCouchCharacterSettings;
class UAnimationManager;

UCLASS()
class COUCHGAME_API ACouchCharacter : public ACharacter, public ICouchDamageable
{
	GENERATED_BODY()
#pragma region Unreal Default
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ACouchCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	APlayerController* GetPlayerController();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetPlayerIndex();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetCurrentTeam();
	UFUNCTION(BlueprintCallable)
	void SetCurrentTeam(int NewTeam);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCouchWidgetSpawn>WidgetSpawner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> WidgetPose;
	
private:
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1, ClampMax = 2))
	int CurrentTeam = 1;

	UPROPERTY()
	APlayerController* PlayerController;
	int PlayerIndex;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TSubclassOf<ACouchWidget3D> FishingWidget;
#pragma endregion
#pragma region Move And Orient
public:
	virtual ACouchPlank* Hit_Implementation(FHitResult HitResult, float RepairingTime, float Scale ) override;
	
	FVector2D GetOrient() const;

	void SetOrient(FVector2D NewOrient);

	void MoveInDirectionOfRotation(float InputStrength);

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector2D Orient = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere)
	float CharacterRotationSpeed = 20.0f;
	void RotateMeshUsingOrient(float DeltaTime) const;
	bool CanMove = true;

	UPROPERTY(EditAnywhere)
	float StunDelay = 2.f;
	void OnTimerStunEnd();

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

	void OnInputInteract();
	
private :
	void BindInputInteractAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputFire(const FInputActionValue& InputActionValue);
	float InputFireValue = 0.f;

	void OnInputMoveInteracting(const FInputActionValue& InputActionValue);
#pragma endregion
#pragma region Hold Item

public:
	bool GetIsHoldingItem() const;

	void OnInputHold(const FInputActionValue& InputActionValue);
private:
	bool IsHoldingItem;
#pragma endregion
#pragma region Fishing
private:
	UPROPERTY()
	TObjectPtr<ACouchFishingRod> FishingRod;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TSubclassOf<ACouchFishingRod> FishingRodSpawn;
	bool isFishing = false;
	bool CanFish = false;
	bool DontFish = false;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> FishingZoneDetectionBox;

	UFUNCTION()
	void OnCharacterBeginOverlapFishingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
											const FHitResult& SweepResult);
	UFUNCTION()
	void OnCharacterEndOverlapFishingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	TObjectPtr<ACouchFishingRod> GetFishingRod() const;
	void DestroyFishingRod();
#pragma endregion
#pragma region Item Spawner
	public:
	UPROPERTY()
	TObjectPtr<AItemSpawnerManager> SpawnerManager;
#pragma endregion
#pragma region Animation Manager
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCouchCharacterAnimationManager* AnimationManager;
#pragma endregion

#pragma region Widget

private:
	void BindInputWidget(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputPause(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UCouchWidgetPause> WidgetPause;

	UCouchWidgetPause* GetFirstWidgetOfClass(TSubclassOf<UCouchWidgetPause> WidgetClass);
	
#pragma endregion	
};
