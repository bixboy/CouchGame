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
#include "Interactables/CoucheCannon.h"
#include "Interfaces/CouchInteractable.h"


// Sets default values
ACouchCharacter::ACouchCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionZone = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionZone"));
	InteractionZone->SetupAttachment(GetMesh());

	InteractionZone->OnComponentBeginOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterBeginOverlap);
	InteractionZone->OnComponentEndOverlap.AddDynamic(this, &ACouchCharacter::OnCharacterEndOverlap);
	
}


// Called when the game starts or when spawned
void ACouchCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();

	InitStateMachine();
	CharacterSettings = GetDefault<UCouchCharacterSettings>();
}

// Called every frame
void ACouchCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	if (!IsInteracting) RotateMeshUsingOrient(DeltaTime);
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

FVector2D ACouchCharacter::GetInputMove() const
{
	return InputMove;
}

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

void ACouchCharacter::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UCouchInteractable>())
	{
		if (!IsInInteractingRange || (InteractingActor && GetDistanceTo(OtherActor) < GetDistanceTo(InteractingActor)))
		{
			IsInInteractingRange = true;
			InteractingActor = OtherActor;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Enter InteractingActor Zone");
		}

	}
	
}

void ACouchCharacter::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UCouchInteractable>())
	{
		if (OtherActor == InteractingActor && !IsInteracting)
		{
			IsInInteractingRange = false;
			InteractingActor = nullptr;
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Exit InteractingActor Zone");
		}
	}
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

void ACouchCharacter::OnInputInteract(const FInputActionValue& InputActionValue)
{
	if (!InteractingActor || !InteractingActor->Implements<UCouchInteractable>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "No valid interactable actor in range.");
		return;
	}

	float ActionValue = InputActionValue.Get<float>();

	if (!IsInInteractingRange)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Out of interaction range.");
		return;
	}

	bool bAlreadyUsed = ICouchInteractable::Execute_IsUsedByPlayer(InteractingActor);

	// Début de l'interaction
	if (!IsInteracting && ActionValue > 0.1f && !bAlreadyUsed)
	{
		StateMachine->ChangeState(ECouchCharacterStateID::InteractingObject);
		IsInteracting = true;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Interacting with actor.");
	}
	// Fin de l'interaction
	else if (IsInteracting && ActionValue < 0.1f)
	{
		StateMachine->ChangeState(ECouchCharacterStateID::Idle);
		IsInteracting = false;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Stopped interacting.");
	}
	// Conditions d'échec d'interaction
	else if (bAlreadyUsed)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Actor is already in use by another player.");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Failed to interact: conditions not met.");
	}
}


void ACouchCharacter::OnInputFire(const FInputActionValue& InputActionValue)
{
	if (IsInteracting)
	{
		if (ACoucheCannon* Cannon = Cast<ACoucheCannon>(InteractingActor); Cannon)
		{
			if (FMath::Abs(InputActionValue.Get<float>()) >= CharacterSettings->InputFireThreshold)
			{
				Cannon->StartCharging();
				// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Input Fire Detected");
			}
			else
			{
				Cannon->StopCharging();
				// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Input Fire Undetected");
			}
		}
	}
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

void ACouchCharacter::OnInputMoveInteracting(const FInputActionValue& InputActionValue)
{
	if (IsInInteractingRange && IsInteracting)
	{
		if (UCouchMovement* CouchMovement = InteractingActor->FindComponentByClass<UCouchMovement>())
		{
			if (InputMove != FVector2D::Zero() && FMath::Abs(InputMove.Y) > .8f) CouchMovement->StartMovement(-InputMove.Y);
			else CouchMovement->StopMovement();	
		}
	}
}



