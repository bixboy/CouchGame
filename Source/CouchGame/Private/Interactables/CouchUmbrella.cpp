#include "Interactables/CouchUmbrella.h"
#include "Widget/CouchWidget3D.h"

ACouchUmbrella::ACouchUmbrella()
{
	PrimaryActorTick.bCanEverTick = true;
	ShieldBox = CreateDefaultSubobject<UBoxComponent>("ShieldBox");
	ShieldBox->SetupAttachment(RootComponent);

	DamageSound = CreateDefaultSubobject<USoundBase>(TEXT("DamageSound"));
	BrokeSound = CreateDefaultSubobject<USoundBase>(TEXT("BrokeSound"));

	BoxInteract->OnComponentEndOverlap.AddDynamic(this, &ACouchUmbrella::OnPlayerEndOverlapp);
}

#pragma region Interfaces
void ACouchUmbrella::PickUp_Implementation(ACouchCharacter* player)
{
	ICouchPickable::PickUp_Implementation(player);
}
void ACouchUmbrella::Drop_Implementation()
{
	ICouchPickable::Drop_Implementation();
}
void ACouchUmbrella::InteractWithObject_Implementation(ACouchInteractableMaster* interactable)
{
	ICouchPickable::InteractWithObject_Implementation(interactable);
}
bool ACouchUmbrella::IsPickable_Implementation()
{
	return !IsPlayerRepairing && !CurrentPlayer;
}
void ACouchUmbrella::SetIsPickable_Implementation(bool isPickable)
{
	ICouchPickable::SetIsPickable_Implementation(isPickable);
}

#pragma endregion

// Tick Repairing
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

// Spawn Widget
void ACouchUmbrella::SpawnOrDeSpawnWarningWidget(bool Spawn)
{
	if(!WidgetComponent->GetCurrentWidget() && WarningWidget && Spawn)
	{
		WidgetComponent->SpawnWidget(WarningWidget, WidgetPose);
	}
	else if (!Spawn)
	{
		WidgetComponent->DestroyWidget();
	}
}

// Custom Interaction
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
		return;
	}

	// Démarrer ou arrêter l'interaction
	if (!IsPlayerRepairing && CurrentPv == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Start Repair");
		StartRepair(Player);
	}
	else if (IsPlayerRepairing && Player == GetCurrentPlayer() && CurrentPv == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "End Repair");
		StopRepair();
	}
}

// Start Repair
void ACouchUmbrella::StartRepair(ACouchCharacter* Player)
{
	SetCurrentPlayer(Player);
	SetPlayerIsIn(true);
	SetCanUse(true);
	CurrentPlayer->SetCanMove(false);
	CurrentPlayer->AnimationManager->IsRepairing = true;
	IsPlayerRepairing = true;
}

// Stop Repair
void ACouchUmbrella::StopRepair()
{
	IsPlayerRepairing = false;
	CurrentPlayer->AnimationManager->IsRepairing = false;
	CurrentPlayer->SetCanMove(true);
	RemoveCurrentPlayer();
	SetPlayerIsIn(false);
	SetCanUse(false);
}

void ACouchUmbrella::OnPlayerEndOverlapp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentPlayer)
	{
		FInputActionValue InputActionValue;
		CurrentPlayer->OnInputInteract(InputActionValue);
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
	CurrentPv = FMath::Clamp(CurrentPv - 1, 0, MaxPv);
	SkeletalMesh->SetSkeletalMeshAsset(CurrentPv > 0 ? DamagedMesh : DestroyedMesh);

	if (CurrentPv == 0)
	{
		HandleZeroPv();
	}
}

void ACouchUmbrella::HandleZeroPv()
{
	if (CurrentPlayer)
	{
		CurrentPv = 1;
		FInputActionValue InputActionValue;
		CurrentPlayer->OnInputInteract(InputActionValue);
	}
	CurrentPv = 0;
	PlayFx();
	ShieldBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetInteractWidget(RepairingWidget);
	SetCanUse(false);
	MovementComponent->SetCanMove(false);

	Timer = 0.f;

	
}

int ACouchUmbrella::GetCurrentPv() const
{
	return CurrentPv;
}

#pragma endregion

#pragma region Repairing

void ACouchUmbrella::FinishRepairing()
{
	if (WidgetComponent->GetCurrentWidget())
	{
		WidgetComponent->DestroyWidget();
	}
	
	ShieldBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	SetInteractWidget(InteractWidget);
	SetCanUse(true);
	MovementComponent->SetCanMove(true);
	
	
	SkeletalMesh->SetSkeletalMeshAsset(RepairingMesh);
	if (CurrentPlayer)
	{
		FInputActionValue InputActionValue;
		CurrentPlayer->OnInputHold(InputActionValue);
	}
	CurrentPv = MaxPv;
}

float ACouchUmbrella::GetPercentRepair_Implementation()
{
	return Timer / TimeToRepair;
} 

bool ACouchUmbrella::GetIsInRepair() const
{
	return IsPlayerRepairing;
}

void ACouchUmbrella::SetIsInRepair(bool Value)
{
	IsPlayerRepairing = Value;
}

#pragma endregion




