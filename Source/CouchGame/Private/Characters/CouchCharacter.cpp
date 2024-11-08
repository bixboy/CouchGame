// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame/Public/Characters/CouchCharacter.h"
#include "CouchGame/Public/Characters/CouchCharacterStateMachine.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/CouchCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "Characters/CouchCharacterSettings.h"
#include "Characters/CouchCharactersStateID.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interactables/CouchFishingRod.h"
#include "Interactables/CouchPickableCannonBall.h"
#include "Interfaces/CouchInteractable.h"
#include "Interfaces/CouchPickable.h"

#pragma region Unreal Default
ACouchCharacter::ACouchCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractionZone = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionZone"));
	InteractionZone->SetupAttachment(GetMesh());
	PickUpItemPosition = CreateDefaultSubobject<USceneComponent>(TEXT("PickUpItemPosition"));
	PickUpItemPosition->SetupAttachment(InteractionZone);

	InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterBeginOverlap);
	InteractionZone->OnComponentEndOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterEndOverlap);
	
}


void ACouchCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();

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
	FRotator MeshRotation = GetMesh()->GetRelativeRotation();
	FVector LeftDirection = FRotationMatrix(MeshRotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(LeftDirection, InputStrength, true);
}

void ACouchCharacter::RotateMeshUsingOrient(float DeltaTime) const
{
	if (InputMove.SizeSquared() > 0.0f)
	{
		FRotator DesiredRotation = FRotationMatrix::MakeFromX(FVector(InputMove.X, -InputMove.Y, 0.0f)).Rotator();
		FRotator CurrentRotation = GetMesh()->GetRelativeRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaTime, CharacterRotationSpeed);
		GetMesh()->SetRelativeRotation(NewRotation);
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
	}
}

void ACouchCharacter::OnInputMove(const FInputActionValue& InputActionValue)
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
#pragma endregion
#pragma region Dash
bool ACouchCharacter::GetCanDash() const
{
	return CanDash;
}


void ACouchCharacter::OnInputDash(const FInputActionValue& InputActionValue)
{
	if (CanDash && CanDashAgain && GetMovementComponent()->IsMovingOnGround())
	{
		CanDashAgain = false;
		InputDashEvent.Broadcast(InputMove);
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
				GEngine->AddOnScreenDebugMessage(
					-1,
					3.0f,
					FColor::Red,
					"Enter InteractingActor Zone"
				);
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
					GEngine->AddOnScreenDebugMessage(
						-1,
						3.0f,
						FColor::Red,
						"Exit InteractingActor Zone"
					);
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
	if (InputData->InputFishingRod)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputFishingRod,
			ETriggerEvent::Started,
			this,
			&ACouchCharacter::OnInputFishing
		);
		
		EnhancedInputComponent->BindAction(
			InputData->InputFishingRod,
			ETriggerEvent::Completed,
			this,
			&ACouchCharacter::OnInputFishing
		);
	}
}

// Interact
void ACouchCharacter::OnInputInteract(const FInputActionValue& InputActionValue)
{
	if (InteractingActors.IsEmpty() && !IsInteracting)
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
	float ActionValue = InputActionValue.Get<float>();
	bool bAlreadyUsed = ICouchInteractable::Execute_IsUsedByPlayer(InteractingActor);
	
	if (!IsInteracting && ActionValue > 0.1f && !bAlreadyUsed)
	{
		IsInteracting = true;
		if (InteractingActor->Implements<UCouchPickable>() && !IsHoldingItem)
		{
			IsHoldingItem = true;
			ICouchInteractable::Execute_Interact(InteractingActor, this);
		}
		else if (!IsHoldingItem)
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
		}
		else
		{
			StateMachine->ChangeState(ECouchCharacterStateID::Idle);
		}
		IsInteracting = false;
		InteractingActor = nullptr;
		if (InteractingActors.Num() == 0) IsInInteractingRange = false;

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Stopped interacting.");
	}
	else if (ActionValue > 0.1f && bAlreadyUsed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Actor is already in use by another player.");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Failed to interact: conditions not met.");
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
}

// Interacting
void ACouchCharacter::OnInputMoveInteracting(const FInputActionValue& InputActionValue)
{
	if (IsInInteractingRange && IsInteracting && InteractingActor)
	{
			if (InputMove != FVector2D::Zero() && FMath::Abs(InputMove.Y) > .8f)
				ICouchInteractable::Execute_StartMoveActor(InteractingActor,-InputMove.Y);
			else ICouchInteractable::Execute_StopMoveActor(InteractingActor);	
	}
}

// Fishing Rod
void ACouchCharacter::OnInputFishing(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Input Fire Detected");
	if (!FishingRod)
	{
		FishingRod = GetWorld()->SpawnActor<ACouchFishingRod>(FishingRodSpawn);
		FishingRod->SetOwner(this);
	}
	else
	{
		FishingRod->Destroy();
		FishingRod = GetWorld()->SpawnActor<ACouchFishingRod>(FishingRodSpawn);
		FishingRod->SetOwner(this);
	}
}

#pragma endregion

#pragma region Hold Item
bool ACouchCharacter::GetIsHoldingItem() const
{
	return IsHoldingItem;
}
#pragma endregion
