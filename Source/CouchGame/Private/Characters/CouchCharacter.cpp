// Fill out your copyright notice in the Description page of Project Settings.

#include "CouchGame/Public/Characters/CouchCharacter.h"
#include "CouchGame/Public/Characters/CouchCharacterStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CouchCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CouchCharacterAnimationManager.h"
#include "Characters/CouchCharacterSettings.h"
#include "Characters/CouchCharactersStateID.h"
#include "Components/BoxComponent.h"
#include "Components/Button.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interactables/CouchFishingRod.h"
#include "Interactables/CouchPickableCannonBall.h"
#include "Interfaces/CouchInteractable.h"
#include "Interfaces/CouchPickable.h"
#include "ItemSpawnerManager/ItemSpawnerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/CouchWidgetPause.h"

#pragma region Unreal Default
ACouchCharacter::ACouchCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractionZone = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionZone"));
	InteractionZone->SetupAttachment(GetMesh());
	PickUpItemPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PickUpItemPosition"));
	PickUpItemPosition->SetupAttachment(InteractionZone);
	FishingZoneDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FishingZoneDetectionBox"));
	FishingZoneDetectionBox->SetupAttachment(GetMesh());
	InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterBeginOverlap);
	InteractionZone->OnComponentEndOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterEndOverlap);

	FishingZoneDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterBeginOverlapFishingZone);
	FishingZoneDetectionBox->OnComponentEndOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterEndOverlapFishingZone);
	
	
	//Récupère le SpawnerManager et le mets dans un TObjectPtr
	AActor* SpawnerManagerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AItemSpawnerManager::StaticClass());
	if (!SpawnerManagerActor) return;
	AItemSpawnerManager* SpawnerManagerPtr = Cast<AItemSpawnerManager>(SpawnerManagerActor);
	if (!SpawnerManagerPtr) return;
	SpawnerManager = TObjectPtr<AItemSpawnerManager>(SpawnerManagerPtr);
}

void ACouchCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();
	AnimationManager = NewObject<UCouchCharacterAnimationManager>();
	InitStateMachine();
	CharacterSettings = GetDefault<UCouchCharacterSettings>();
}

void ACouchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	if (!IsInteracting || IsHoldingItem) RotateMeshUsingOrient(DeltaTime);
	if (!CanDashAgain)
	{
		DashTimer += DeltaTime;
		if (DashTimer >= DashDuration)
		{
			DashTimer = 0;
			CanDashAgain = true;
			AnimationManager->IsDashCooldown = false;
		}
	}
}

// Called to bind functionality to input
void ACouchCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) return;

	BindInputMoveAndActions(EnhancedInputComponent);
	BindInputInteractAndActions(EnhancedInputComponent);
	BindInputWidget(EnhancedInputComponent);
}

void ACouchCharacter::Hit_Implementation(FHitResult HitResult, float RepairingTime, float Scale)
{
	ICouchDamageable::Hit_Implementation(HitResult, RepairingTime, Scale);
	CanMove = false;
	
	FTimerHandle RoundTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RoundTimerHandle, this, &ACouchCharacter::OnTimerStunEnd, StunDelay, false);
}

void ACouchCharacter::OnTimerStunEnd()
{
	CanMove = true;
}

#pragma endregion Unreal Default

#pragma region Move And Orient
FVector2D ACouchCharacter::GetOrient() const
{
	return Orient;
}

void ACouchCharacter::SetOrient(FVector2D NewOrient)
{
	Orient = NewOrient;
}

void ACouchCharacter::MoveInDirectionOfRotation(float InputStrength)
{
	FVector LeftDirection = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(LeftDirection, InputStrength, true);
}

void ACouchCharacter::RotateMeshUsingOrient(float DeltaTime) const
{
	if (InputMove.SizeSquared() > 0.0f)
	{
		// Calculate the desired rotation based on input
		FRotator DesiredRotation = FRotationMatrix::MakeFromX(FVector(InputMove.X, -InputMove.Y, 0.0f)).Rotator();

		// Interpolate between the current control rotation and the desired rotation
		if (Controller)
		{
			FRotator CurrentRotation = Controller->GetControlRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, CharacterRotationSpeed);

			// Apply the new rotation to the controller
			Controller->SetControlRotation(NewRotation);
		}

		// Synchronize the mesh's rotation with the actor's current rotation
		GetMesh()->SetRelativeRotation(FRotator::ZeroRotator);
	}
}
#pragma endregion

#pragma region State Machine

void ACouchCharacter::CreateStateMachine()
{
	StateMachine = NewObject<UCouchCharacterStateMachine>(this);
}

void ACouchCharacter::InitStateMachine()
{
	if (!StateMachine) return;
	StateMachine->Init(this);
}

void ACouchCharacter::TickStateMachine(float DeltaTime) const
{
	if (!StateMachine) return;
	StateMachine->Tick(DeltaTime);
}
#pragma endregion

#pragma region InputData / MappingContext
void ACouchCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (!PlayerController) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (!Player) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!InputSystem) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}
#pragma endregion

#pragma region InputMove
FVector2D ACouchCharacter::GetInputMove() const
{
	return InputMove;
}


void ACouchCharacter::BindInputMoveAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!InputData) return;

	if(InputData->InputActionMove)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMove,
			ETriggerEvent::Started,
			this,
			&ACouchCharacter::OnInputMove
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMove,
			ETriggerEvent::Started,
			this,
			&ACouchCharacter::OnInputMoveInteracting
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMove,
			ETriggerEvent::Completed,
			this,
			&ACouchCharacter::OnInputMove
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMove,
			ETriggerEvent::Completed,
			this,
			&ACouchCharacter::OnInputMoveInteracting
		);

		EnhancedInputComponent->BindAction(
			InputData->InputActionMove,
			ETriggerEvent::Triggered,
			this,
			&ACouchCharacter::OnInputMove
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMove,
			ETriggerEvent::Triggered,
			this,
			&ACouchCharacter::OnInputMoveInteracting
		);
	}

	if (InputData->InputActionDash)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionDash,
			ETriggerEvent::Started,
			this,
			&ACouchCharacter::OnInputDash
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionDash,
			ETriggerEvent::Completed,
			this,
			&ACouchCharacter::OnInputDash
		);
	}
}

void ACouchCharacter::OnInputMove(const FInputActionValue& InputActionValue)
{
	if (CanMove)
	{
		FVector2D Input = InputActionValue.Get<FVector2D>();
		if (FMath::Abs(Input.X )>= CharacterSettings->InputMoveThreshold || FMath::Abs(Input.Y)>= CharacterSettings->InputMoveThreshold)
		{
			InputMove = Input;
		}
		else
		{
			InputMove = FVector2D::Zero();
		}	
	}
}
#pragma endregion

#pragma region Dash
bool ACouchCharacter::GetCanDash() const
{
	return CanDash;
}


void ACouchCharacter::OnInputDash(const FInputActionValue& InputActionValue)
{
	if (InputActionValue.Get<float>() > 0.1f)
	{
		AnimationManager->HasPressedDashInput = true;
		if (CanDash && CanDashAgain && GetMovementComponent()->IsMovingOnGround())
		{
			CanDashAgain = false;
			AnimationManager->IsDashCooldown = true;
			InputDashEvent.Broadcast(InputMove);
		}
	}
	else
	{
		AnimationManager->HasPressedDashInput = false;
	}
	
	
}

#pragma endregion

#pragma region Interraction
void ACouchCharacter::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UCouchInteractable>())
	{
		if (TObjectPtr<ACouchInteractableMaster> Actor = Cast<ACouchInteractableMaster>(OtherActor); Actor)
		{
			InteractingActors.Add(Actor);
			if (!IsInInteractingRange)
			{
				IsInInteractingRange = true;
				//GEngine->AddOnScreenDebugMessage(
				//	-1,
				//	3.0f,
				//	FColor::Red,
				//	"Enter InteractingActor Zone"
				//);
			}
		}


	}
	
}

void ACouchCharacter::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UCouchInteractable>())
	{
		if (InteractingActors.Contains(OtherActor))
		{
			if (TObjectPtr<ACouchInteractableMaster> Actor = Cast<ACouchInteractableMaster>(OtherActor); Actor)
			{
				InteractingActors.Remove(Actor);
				if (InteractingActors.Num() == 0 && !IsInteracting && !InteractingActor)
				{
					IsInInteractingRange = false;
					//GEngine->AddOnScreenDebugMessage(
					//	-1,
					//	3.0f,
					//	FColor::Red,
					//	"Exit InteractingActor Zone"
					//);
				}
			}
		}
	}
}

TObjectPtr<ACouchInteractableMaster> ACouchCharacter::FindNearestInteractingActor() const
{
	float minDistanceFromPlayer = 10000;
	TObjectPtr<ACouchInteractableMaster> nearestInteractingActor = nullptr;
	for (auto InteractActor : InteractingActors)
	{
		if (!InteractActor) continue;
		float DistanceToPlayer = GetDistanceTo(InteractActor);
		if(DistanceToPlayer < minDistanceFromPlayer)
		{
			nearestInteractingActor = InteractActor;
			minDistanceFromPlayer = DistanceToPlayer;
		}
	}
	return nearestInteractingActor;
}

void ACouchCharacter::BindInputInteractAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!InputData) return;
	if (InputData->InputActionInteract)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionInteract,
			ETriggerEvent::Started,
			this,
			&ACouchCharacter::OnInputInteract
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionInteract,
			ETriggerEvent::Completed,
			this,
			&ACouchCharacter::OnInputInteract
		);
	}
	if (InputData->InputActionFire)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionFire,
			ETriggerEvent::Started,
			this,
			&ACouchCharacter::OnInputFire
		);
		// EnhancedInputComponent->BindAction(
		// 	InputData->InputActionFire,
		// 	ETriggerEvent::Triggered,
		// 	this,
		// 	&ACouchCharacter::OnInputFire
		// );
		
		EnhancedInputComponent->BindAction(
			InputData->InputActionFire,
			ETriggerEvent::Completed,
			this,
			&ACouchCharacter::OnInputFire
		);
	}
}

// Interact
void ACouchCharacter::OnInputInteract(const FInputActionValue& InputActionValue)
{
	float ActionValue = InputActionValue.Get<float>();
	if (isFishing && FishingRod)
	{
		if (ActionValue > 0)
		{
			FishingRod->isPlayerFishing = true;
		}
		else
		{
			FishingRod->isPlayerFishing = false;
			FishingRod->StopRewindCable();
		}
		return;
	}
	
	if ((InteractingActors.IsEmpty() && !IsInteracting) || isFishing)
	{
		return;
	}
	
	if (!InteractingActor)
	{
		InteractingActor = FindNearestInteractingActor();
		if (!InteractingActor)
		{
			return;
		}
	}
	
	bool bAlreadyUsed = ICouchInteractable::Execute_IsUsedByPlayer(InteractingActor);
	
	if (!IsInteracting && ActionValue > 0.1f && !bAlreadyUsed)
	{
		IsInteracting = true;
		if (InteractingActor->Implements<UCouchPickable>() && !IsHoldingItem)
		{
			IsHoldingItem = true;
			ICouchInteractable::Execute_Interact(InteractingActor, this);
		}
		else if (!InteractingActor->Implements<UCouchPickable>() && !IsHoldingItem)
		{
			StateMachine->ChangeState(ECouchCharacterStateID::InteractingObject);
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Interacting with actor.");
	}
	else if (IsInteracting && ActionValue < 0.1f)
	{
		if (IsHoldingItem)
		{
			if (TObjectPtr<ACouchPickableMaster> PickableItem = Cast<ACouchPickableMaster>(InteractingActor); PickableItem)
			{
				TObjectPtr<ACouchInteractableMaster> ItemToInteractWith =
					PickableItem->PlayerCanUsePickableItemToInteract(PickableItem, InteractingActors);
				if (ItemToInteractWith)
				{
					ICouchPickable::Execute_InteractWithObject(PickableItem, ItemToInteractWith.Get());
				}
			}
			ICouchInteractable::Execute_Interact(InteractingActor, this);
			IsHoldingItem = false;
			AnimationManager->IsCarryingItem = false;
		}
		else
		{
			StateMachine->ChangeState(ECouchCharacterStateID::Idle);
		}
		IsInteracting = false;
		InteractingActor = nullptr;
		if (InteractingActors.Num() == 0) IsInInteractingRange = false;

		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Stopped interacting.");
	}
	else if (ActionValue > 0.1f && bAlreadyUsed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Actor is already in use by another player.");
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Failed to interact: conditions not met.");
	}
}

// Fire
void ACouchCharacter::OnInputFire(const FInputActionValue& InputActionValue)
{
	if (IsInteracting)
	{
		if (!InteractingActor->Implements<UCouchInteractable>()) return;
		{
			if (FMath::Abs(InputActionValue.Get<float>()) >= CharacterSettings->InputFireThreshold)
			{
				ICouchInteractable::Execute_StartChargeActor(InteractingActor);
				//Cannon->StartCharging();
				//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Input Fire Detected");
			}
			else
			{
				ICouchInteractable::Execute_StopChargeActor(InteractingActor);
				//Cannon->StopCharging();
				// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Input Fire Undetected");
			}
		}
	}
	else if (CanFish)
	{
		if (FMath::Abs(InputActionValue.Get<float>()) >= CharacterSettings->InputFireThreshold)
		{
			if (!FishingRod)
			{
				FishingRod = GetWorld()->SpawnActor<ACouchFishingRod>(FishingRodSpawn);
				FishingRod->SetupFishingRod(this, CurrentTeam);
				isFishing = true;
				CanMove = false;
				InputMove = FVector2D::ZeroVector;
				StateMachine->ChangeState(ECouchCharacterStateID::Idle);
				ICouchInteractable::Execute_StartChargeActor(FishingRod);
			}
			else
			{
				isFishing = true;
				CanMove = false;
				InputMove = FVector2D::ZeroVector;
				StateMachine->ChangeState(ECouchCharacterStateID::Idle);
				ICouchInteractable::Execute_StartChargeActor(FishingRod);
			}
		}
		else
		{
			if (FishingRod)
			{
				ICouchInteractable::Execute_StopChargeActor(FishingRod);
			}
			CanMove = true;
		}
	}
}

// Interacting
void ACouchCharacter::OnInputMoveInteracting(const FInputActionValue& InputActionValue)
{
	if (IsInInteractingRange && IsInteracting && InteractingActor && !FishingRod)
	{
			if (InputMove != FVector2D::Zero() && FMath::Abs(InputMove.Y) > .8f)
				ICouchInteractable::Execute_StartMoveActor(InteractingActor,-InputMove.Y);
			else ICouchInteractable::Execute_StopMoveActor(InteractingActor);	
	}
}

#pragma endregion

#pragma region Hold Item
bool ACouchCharacter::GetIsHoldingItem() const
{
	return IsHoldingItem;
}
#pragma endregion

#pragma region Fishing

// Fishing
void ACouchCharacter::OnCharacterBeginOverlapFishingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CanFish = true;
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Fishing");
}

void ACouchCharacter::OnCharacterEndOverlapFishingZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CanFish = false;
	DestroyFishingRod();
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Not Fishing");
}

TObjectPtr<ACouchFishingRod> ACouchCharacter::GetFishingRod() const
{
	return FishingRod;
}

void ACouchCharacter::DestroyFishingRod()
{
	if (FishingRod)
	{
		FishingRod->DestroyFishingRod();
		FishingRod->Destroy();
		FishingRod = nullptr;
	}
	isFishing = false;
	AnimationManager->IsFishingStart = false;
	AnimationManager->IsFishingRelease = false;
	AnimationManager->IsFishingPull = false;
	AnimationManager->IsFishing = false;
}
#pragma endregion

#pragma region Widget

	void ACouchCharacter::BindInputWidget(UEnhancedInputComponent* EnhancedInputComponent)
	{
		if (!InputData) return;
		if (InputData->InputActionPause)
		{
			EnhancedInputComponent->BindAction(
				InputData->InputActionPause,
				ETriggerEvent::Started,
				this,
				&ACouchCharacter::OnInputPause
			);
		}
	}

void ACouchCharacter::OnInputPause(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Red,
		"Enter InteractingActor Zone"
	);
	if (!WidgetRef)
	{
		WidgetRef = CreateWidget<UCouchWidgetPause>(GetWorld(), WidgetPause);
		if (WidgetRef)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(WidgetRef->Btn_Resume->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
			WidgetRef->AddToViewport();
		}	
	}
	else if (WidgetRef)
	{
		WidgetRef->RemoveFromParent();
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	}
}

#pragma endregion
