#include "Interactables/CouchUmbrella.h"

#include "Kismet/GameplayStatics.h"

ACouchUmbrella::ACouchUmbrella()
{
	PrimaryActorTick.bCanEverTick = true;
	ShieldBox = CreateDefaultSubobject<UBoxComponent>("ShieldBox");
	ShieldBox->SetupAttachment(RootComponent);

	DamageSound = CreateDefaultSubobject<USoundBase>(TEXT("DamageSound"));
	BrokeSound = CreateDefaultSubobject<USoundBase>(TEXT("BrokeSound"));
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
	}
}

void ACouchUmbrella::Interact_Implementation(ACouchCharacter* Player)
{
	// Si un autre joueur utilise déjà la planche, empêcher l'interaction
	if (IsPlayerRepairing && Player != GetCurrentPlayer() && CurrentPv == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Already in use by another player");
		return;
	}
	
	// Execute le parent
	if (CurrentPv > 0)
	{
		Super::Interact_Implementation(Player);	
	}
	
	// Démarrer ou arrêter l'interaction
	if (!IsPlayerRepairing && CurrentPv == 0)
	{
		SetCurrentPlayer(Player);
		SetPlayerIsIn(true);
		SetCanUse(true);
		IsPlayerRepairing = true;
	}
	else if (IsPlayerRepairing && Player == GetCurrentPlayer() && CurrentPv == 0)
	{
		IsPlayerRepairing = false;
		RemoveCurrentPlayer();
		SetPlayerIsIn(false);
		SetCanUse(false);
	}
}

#pragma region Life

ACouchPlank* ACouchUmbrella::Hit_Implementation(FHitResult HitResult, float RepairingTime, float Scale)
{
	ICouchDamageable::Hit_Implementation(HitResult, RepairingTime, Scale);
	PlayFx();
	DecreasePv();
	return nullptr;
}

void ACouchUmbrella::DecreasePv()
{
	CurrentPv --;
	CurrentPv = FMath::Clamp(CurrentPv, 0, MaxPv);
	SkeletalMesh->SetSkeletalMeshAsset(DamagedMesh);
	
	if (CurrentPv == 0)
	{
		PlayFx();
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
	SetInteractWidget();
	if (WidgetComponent->GetCurrentWidget()){ WidgetComponent->DestroyWidget();}
	
	SkeletalMesh->SetSkeletalMeshAsset(RepairingMesh);
	CurrentPv = MaxPv;
}

float ACouchUmbrella::GetPercentRepair_Implementation()
{
	return Timer / TimeToRepair;
}

#pragma endregion




