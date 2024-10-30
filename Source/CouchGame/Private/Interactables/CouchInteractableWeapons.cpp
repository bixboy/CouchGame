// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchInteractableWeapons.h"

ACouchInteractableWeapons::ACouchInteractableWeapons()
{
	PrimaryActorTick.bCanEverTick = true;
	Setup();
}

void ACouchInteractableWeapons::BeginPlay()
{
	Super::BeginPlay();
}

void ACouchInteractableWeapons::Setup()
{
	WidgetComponent = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("SpawnerWidget"));
	MovementComponent = CreateDefaultSubobject<UCouchMovement>(TEXT("MovementComponent"));

	BoxInteract = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxInteract"));
	RootComponent = BoxInteract;
	
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	WidgetPose = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetPose"));
	WidgetPose->SetupAttachment(RootComponent);
	PlayerPose = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPose"));
	PlayerPose->SetupAttachment(RootComponent);
}

bool ACouchInteractableWeapons::GetCanUse() const {return CanUse;}

void ACouchInteractableWeapons::SetCanUse(bool Value) {CanUse = Value;}

void ACouchInteractableWeapons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACouchInteractableWeapons::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Appel de l'interface Interact dans la classe parent."));

	if (!GetCurrentPlayer() || GetCurrentPlayer() == Player)
	{
		if (!Execute_IsUsedByPlayer(this))
		{
			CanUse = true;
			
			FTransform PoseTransform = FTransform(PlayerPose->GetComponentRotation(), PlayerPose->GetComponentLocation(), GetCurrentPlayer()->GetActorScale());
			GetCurrentPlayer()->SetActorTransform(PoseTransform, false);
			GetCurrentPlayer()->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		else
		{
			WidgetComponent->DestroyWidget();
			
			SetPlayerIsIn(false);
			SetCurrentPlayer(nullptr);
			CanUse = false;
			
			StopMovement();
			Player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}	
	}
}

#pragma region MovementComponent

void ACouchInteractableWeapons::StartMovement(int InputDirection)
{
	if (Execute_IsUsedByPlayer(this))
	{
		CanUse = false;
		MovementComponent->StartMovement(InputDirection);
	}
}

void ACouchInteractableWeapons::StopMovement()
{
	if (Execute_IsUsedByPlayer(this))
	{
		CanUse = true;
		MovementComponent->StopMovement();	
	}
	else
	{
		MovementComponent->StopMovement();
	}
}

#pragma endregion

