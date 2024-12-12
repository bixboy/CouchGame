// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchInteractableWeapons.h"
#include "Widget/CouchWidget3D.h"

ACouchInteractableWeapons::ACouchInteractableWeapons()
{
	PrimaryActorTick.bCanEverTick = true;
	Setup();
}

#pragma region Setup

void ACouchInteractableWeapons::BeginPlay()
{
	Super::BeginPlay();
	SetInteractWidget();
}

void ACouchInteractableWeapons::Setup()
{
	WidgetComponent = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("SpawnerWidget"));
	MovementComponent = CreateDefaultSubobject<UCouchMovement>(TEXT("MovementComponent"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	BoxInteract = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxInteract"));
	BoxInteract->SetupAttachment(SkeletalMesh);
	PlayerPose = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPose"));
	PlayerPose->SetupAttachment(SkeletalMesh);
	WidgetPose = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetPose"));
	WidgetPose->SetupAttachment(SkeletalMesh);

	BoxInteract->OnComponentBeginOverlap.AddDynamic(this, &ACouchInteractableWeapons::OnCharacterBeginOverlap);
	BoxInteract->OnComponentEndOverlap.AddDynamic(this, &ACouchInteractableWeapons::OnCharacterEndOverlap);
}

bool ACouchInteractableWeapons::GetCanUse() const {return CanUse;}

void ACouchInteractableWeapons::SetCanUse(bool Value) {CanUse = Value;}

void ACouchInteractableWeapons::SetInteractWidget(TSubclassOf<ACouchWidget3D> InteractingWidget)
{
	if (InteractingWidget)
	{
		CurrentInteractWidget = InteractingWidget;	
	}
	else if (!InteractingWidget)
	{
		CurrentInteractWidget = InteractWidget;
	}
}

#pragma endregion

void ACouchInteractableWeapons::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
	
	if (!GetCurrentPlayer() || GetCurrentPlayer() == Player)
	{
		WidgetComponent->DestroyWidget();
		if (!Execute_IsUsedByPlayer(this))
		{
			SetPlayerIsIn(true);
			CanUse = true;
			if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDragging = true;
			FTransform PoseTransform = FTransform(PlayerPose->GetComponentRotation(), PlayerPose->GetComponentLocation(), GetCurrentPlayer()->GetActorScale());
			GetCurrentPlayer()->SetActorTransform(PoseTransform, false);
			GetCurrentPlayer()->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);	
		}
		else
		{
			WidgetComponent->SpawnWidget(CurrentInteractWidget, WidgetPose);
			WidgetComponent->SpawnWidget(InteractWidget, WidgetPose);
			
			DetachPlayer(Player);
		}	
	}
}

void ACouchInteractableWeapons::DetachPlayer(ACouchCharacter* Player)
{
	if (Player)
	{
		Player->AnimationManager->IsDragging = false;
		Player->AnimationManager->IsDraggingForward = false;
		Player->AnimationManager->IsDraggingBackward = false;
		Player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
			
	RemoveCurrentPlayer();
	CanUse = false;
	SetPlayerIsIn(false);
	Execute_StopMoveActor(this);
}

#pragma region MovementComponent

void ACouchInteractableWeapons::StartMoveActor_Implementation(int InputDirection)
{
	Super::StartMoveActor_Implementation(InputDirection);
	if (Execute_IsUsedByPlayer(this) && MovementComponent->GetCanMove())
	{
		int Direction = InputDirection;
		if (InvertInputMove)
		{
			Direction = InputDirection * -1.f;	
		}
		PlayVibration(MoveVibrationEffect);
		CanUse = true;
		MovementComponent->StartMovement(Direction);
		if (Direction == 1)
		{
			if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingForward = true;
			if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingBackward = false;
		}
		else
		{
			if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingForward = false;
			if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingBackward = true;
		}
	}
}

void ACouchInteractableWeapons::StopMoveActor_Implementation()
{
	Super::StopMoveActor_Implementation();
	if (Execute_IsUsedByPlayer(this))
	{
		CanUse = true;
		MovementComponent->StopMovement();
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingForward = false;
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingBackward = false;
	}
	else
	{
		MovementComponent->StopMovement();
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingForward = false;
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsDraggingBackward = false;
	}
}

#pragma endregion

#pragma region Overlap

void ACouchInteractableWeapons::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Execute_IsUsedByPlayer(this))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Enter InteractingActor Zone");
		WidgetComponent->SpawnWidget(CurrentInteractWidget, WidgetPose);	
	}
}

void ACouchInteractableWeapons::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Execute_IsUsedByPlayer(this))
		WidgetComponent->DestroyWidget();
}

#pragma endregion







