// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchShield.h"

ACouchShield::ACouchShield()
{
	PrimaryActorTick.bCanEverTick = true;
	SetupShield();
}

void ACouchShield::SetupShield()
{
	WidgetComponent = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("SpawnerWidget"));
	MovementComponent = CreateDefaultSubobject<UCouchMovement>(TEXT("MovementComponent"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	WidgetPose = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetPose"));
	WidgetPose->SetupAttachment(RootComponent);
	PlayerPose = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPose"));
	PlayerPose->SetupAttachment(RootComponent);
}

void ACouchShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACouchShield::Interact_Implementation(ACouchCharacter* Player)
{
	if (!CurrentPlayer || CurrentPlayer == Player)
	{
		if (!PlayerIsIn)
		{
			CurrentPlayer = Player;
			PlayerIsIn = true;
		
			FTransform PoseTransform = FTransform(PlayerPose->GetComponentRotation(), PlayerPose->GetComponentLocation(), Player->GetActorLocation());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PlayerPose Location: %s"), *PoseTransform.GetLocation().ToString()));

			Player->SetActorTransform(PoseTransform, false);
			Player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		else
		{
			PlayerIsIn = false;
			WidgetComponent->DestroyWidget();
			StopMovement();
			CurrentPlayer = nullptr;
			Player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}	
	}
}

#pragma region Movement

void ACouchShield::StartMovement(int InputDirection)
{
	if (PlayerIsIn)
	{
		MovementComponent->StartMovement(InputDirection);
	}
}

void ACouchShield::StopMovement()
{
		MovementComponent->StopMovement();	
	
}

#pragma endregion

