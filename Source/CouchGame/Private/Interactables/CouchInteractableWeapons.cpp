// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchInteractableWeapons.h"

ACouchInteractableWeapons::ACouchInteractableWeapons()
{
	PrimaryActorTick.bCanEverTick = true;
	Setup();
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

	BoxInteract->OnComponentBeginOverlap.AddDynamic(this, &ACouchInteractableWeapons::OnCharacterBeginOverlap);
	BoxInteract->OnComponentEndOverlap.AddDynamic(this, &ACouchInteractableWeapons::OnCharacterEndOverlap);
}

bool ACouchInteractableWeapons::GetCanUse() const {return CanUse;}

void ACouchInteractableWeapons::SetCanUse(bool Value) {CanUse = Value;}

void ACouchInteractableWeapons::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
	
	if (!GetCurrentPlayer() || GetCurrentPlayer() == Player)
	{
		WidgetComponent->DestroyWidget();
		if (!Execute_IsUsedByPlayer(this))
		{
			CanUse = true;
			SetPlayerIsIn(true);
			
			FTransform PoseTransform = FTransform(PlayerPose->GetComponentRotation(), PlayerPose->GetComponentLocation(), GetCurrentPlayer()->GetActorScale());
			GetCurrentPlayer()->SetActorTransform(PoseTransform, false);
			GetCurrentPlayer()->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		else
		{
			WidgetComponent->SpawnWidget(InteractWidget, WidgetPose);

			RemoveCurrentPlayer();
			CanUse = false;
			SetPlayerIsIn(false);
			
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

void ACouchInteractableWeapons::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Enter InteractingActor Zone");
	if (!Execute_IsUsedByPlayer(this))
		WidgetComponent->SpawnWidget(InteractWidget, WidgetPose);
}

void ACouchInteractableWeapons::OnCharacterEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Execute_IsUsedByPlayer(this))
		WidgetComponent->DestroyWidget();
}

