// Fill out your copyright notice in the Description page of Project Settings.

#include "Boat/CouchPlank.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Boat/BoatFloor.h"
#include "Components/SceneComponent.h"
#include "Widget/CouchWidgetSpawn.h"
#include "Characters/CouchCharacter.h"
#include "Widget/CouchWidget3D.h"

// Sets default values
ACouchPlank::ACouchPlank()
{
	PrimaryActorTick.bCanEverTick = true;
	BlockAll = CreateDefaultSubobject<UBoxComponent>("BlockAll");
	InterractiveBoxRange = CreateDefaultSubobject<UBoxComponent>("InterractiveBoxRange");
	HitMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlankMesh");
	WaterParticle = CreateDefaultSubobject<UNiagaraComponent>("Water");
	WidgetPos = CreateDefaultSubobject<USceneComponent>("WidgetPos");
	CouchWidgetSpawn = CreateDefaultSubobject<UCouchWidgetSpawn>("CouchWidgetSpawn");

	RootComponent = BlockAll;
	HitMesh->SetupAttachment(BlockAll);
	if (UStaticMesh* RandomStaticMesh = GetRandomStaticMesh()) HitMesh->SetStaticMesh(RandomStaticMesh);
	InterractiveBoxRange->SetupAttachment(HitMesh);
	WaterParticle->SetupAttachment(HitMesh);
	WidgetPos->SetupAttachment(HitMesh);

	InterractiveBoxRange->OnComponentBeginOverlap.AddDynamic(this, &ACouchPlank::OnOverlapBegin);
	InterractiveBoxRange->OnComponentEndOverlap.AddDynamic(this, &ACouchPlank::OnOverlapEnd);
}

void ACouchPlank::Init(ABoatFloor* floor, float RepairingTime, float Scale)
{
	Floor = floor;
	if (RepairingTime > 0) TimeToRepair *= RepairingTime;
	if (Scale > 0) this->SetActorScale3D(GetActorScale()*Scale);
}

bool ACouchPlank::IsUsedByPlayer_Implementation()
{
	ICouchInteractable::IsUsedByPlayer_Implementation();
	return IsPlayerRepairing;
}

void ACouchPlank::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACouchCharacter::StaticClass()) && !IsPlayerRepairing)
	{
		UClass* InteractWidget = InteractWidgetClass.Get();
		CouchWidgetSpawn->SpawnWidget(InteractWidget, WidgetPos);
		InteractWidgetPtr = CouchWidgetSpawn->GetCurrentWidget();
	}
}

void ACouchPlank::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ACouchCharacter::StaticClass()) && !IsPlayerRepairing)
	{
		if (CouchWidgetSpawn->GetCurrentWidget()) CouchWidgetSpawn->DestroyWidget();
	}
}

float ACouchPlank::GetRepairingPercent() const
{
	return Timer / TimeToRepair;
}

AActor* ACouchPlank::GetInteractWidget() const
{
	return InteractWidgetPtr;
}

void ACouchPlank::BeginPlay()
{
	Super::BeginPlay();
}

void ACouchPlank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Si le joueur est en train de réparer
	if (IsPlayerRepairing)
	{
		Timer += DeltaTime;
		if (Timer >= TimeToRepair)
		{
			Floor->RemoveHitFromArray(this);
			if (CouchWidgetSpawn->GetCurrentWidget()) CouchWidgetSpawn->DestroyWidget();
			// APlayer->IsInteracting = false;
			CurrentPlayer->AnimationManager->IsRepairing = false;
			Destroy();
		}
	}
	else if (Timer > 0)
	{
		Timer = FMath::Clamp(Timer - DeltaTime, 0, TimeToRepair);
	}
}

UStaticMesh* ACouchPlank::GetRandomStaticMesh()
{
	if (DamagedPlanckMeshes.Num() == 0) return nullptr;
	int RandomIndex = FMath::RandRange(0, DamagedPlanckMeshes.Num() - 1);
	return DamagedPlanckMeshes[RandomIndex];
}

void ACouchPlank::Interact_Implementation(ACouchCharacter* Player)
{
	// Si un autre joueur utilise déjà la planche, empêcher l'interaction
	if (IsPlayerRepairing && Player != APlayer)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Already in use by another player");
		return;
	}

	ICouchInteractable::Interact_Implementation(Player);

	// Démarrer ou arrêter l'interaction
	if (!IsPlayerRepairing)
	{
		APlayer = Player;
		IsPlayerRepairing = true;
		Player->AnimationManager->IsRepairing = true;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Player started interacting");
	}
	else if (IsPlayerRepairing && Player == APlayer)
	{
		APlayer = nullptr;
		Player->AnimationManager->IsRepairing = false;
		IsPlayerRepairing = false;
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, "Player stopped interacting");
	}
}
