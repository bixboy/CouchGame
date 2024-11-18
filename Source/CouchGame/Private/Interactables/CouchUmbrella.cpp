// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchUmbrella.h"

ACouchUmbrella::ACouchUmbrella()
{
	PrimaryActorTick.bCanEverTick = true;
	ShieldBox = CreateDefaultSubobject<UBoxComponent>("ShieldBox");
	ShieldBox->SetupAttachment(RootComponent);
}

void ACouchUmbrella::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPlayerRepairing)
	{
		Timer += DeltaTime;
		if (Timer >= TimeToRepair)
		{
			FinishRepairing();
		}
	}
	else if (Timer > 0)
	{
		Timer = FMath::Clamp(Timer - DeltaTime, 0, TimeToRepair);
		SetCanUse(false);
	}
}

void ACouchUmbrella::Interact_Implementation(ACouchCharacter* Player)
{
	// Si un autre joueur utilise déjà la planche, empêcher l'interaction
	if (IsPlayerRepairing && Player != GetCurrentPlayer())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Already in use by another player");
		return;
	}
	
	// Execute le parent
	Super::Interact_Implementation(Player);

	// Démarrer ou arrêter l'interaction
	if (!IsPlayerRepairing && CurrentPv == 0)
	{
		IsPlayerRepairing = true;
	}
	else if (IsPlayerRepairing && Player == GetCurrentPlayer() && CurrentPv == 0)
	{
		IsPlayerRepairing = false;
	}
}

#pragma region Life

void ACouchUmbrella::Hit_Implementation(FHitResult HitResult, float RepairingTime, float Scale)
{
	ICouchDamageable::Hit_Implementation(HitResult, RepairingTime, Scale);
	DecreasePv();
}

void ACouchUmbrella::DecreasePv()
{
	CurrentPv --;
	CurrentPv = FMath::Clamp(CurrentPv, 0, MaxPv);
	SkeletalMesh->SetSkeletalMeshAsset(DamagedMesh);
	
	if (CurrentPv == 0)
	{
		SetCanUse(false);
		ShieldBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		SkeletalMesh->SetSkeletalMeshAsset(DestroyedMesh);
		SetInteractWidget(RepairingWidget);
	}
}

int ACouchUmbrella::GetCurrentPv() const
{
	return CurrentPv;
}

#pragma endregion

#pragma region Repairing

void ACouchUmbrella::FinishRepairing()
{
	if (WidgetComponent->GetCurrentWidget()) WidgetComponent->DestroyWidget();
	SetCanUse(true);
	RemoveCurrentPlayer();
	SkeletalMesh->SetSkeletalMeshAsset(RepairingMesh);
	CurrentPv = MaxPv;
	SetInteractWidget();
}

float ACouchUmbrella::GetPercentRepair_Implementation()
{
	return Timer / TimeToRepair;
}

#pragma endregion




