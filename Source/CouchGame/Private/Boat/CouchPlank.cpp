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
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BlockAll = CreateDefaultSubobject<UBoxComponent>("BlockAll");
	InterractiveBoxRange = CreateDefaultSubobject<UBoxComponent>("InterractiveBoxRange");
	HitMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlankMesh");
	WaterParticle = CreateDefaultSubobject<UNiagaraComponent>("Water");
	WidgetPos = CreateDefaultSubobject<USceneComponent>("WidgetPos");
	CouchWidgetSpawn = CreateDefaultSubobject<UCouchWidgetSpawn>("CouchWidgetSpawn");
	
	RootComponent = BlockAll;
	HitMesh->SetupAttachment(BlockAll);
	if (UStaticMesh* RandomStaticMesh = GetRandomStaticMesh()) HitMesh->SetStaticMesh(RandomStaticMesh); // Je mets un mesh aléatoire
	InterractiveBoxRange->SetupAttachment(HitMesh);
	WaterParticle->SetupAttachment(HitMesh);
	WidgetPos->SetupAttachment(HitMesh);

	InterractiveBoxRange->OnComponentBeginOverlap.AddDynamic(this, &ACouchPlank::OnOverlapBegin);
	InterractiveBoxRange->OnComponentEndOverlap.AddDynamic(this, &ACouchPlank::OnOverlapEnd);
}

void ACouchPlank::Init(ABoatFloor* floor)
{
	Floor = floor;
}

void ACouchPlank::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACouchCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Les méchants");
		UClass* InteractWidget = InteractWidgetClass.Get();
		CouchWidgetSpawn->SpawnWidget(InteractWidget, WidgetPos);
		InteractWidgetPtr = CouchWidgetSpawn->GetCurrentWidget();
		if (InteractWidgetPtr)
		{
			
		}
	}
}

void ACouchPlank::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ACouchCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "Les gentils");
		CouchWidgetSpawn->DestroyWidget();
	}
	
}

float ACouchPlank::GetRepairingPercent() const
{
	return Timer/TimeToRepair;
}

AActor* ACouchPlank::GetInteractWidget() const
{
	return InteractWidgetPtr;
}

// Called when the game starts or when spawned
void ACouchPlank::BeginPlay()
{
	Super::BeginPlay();
}

void ACouchPlank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsPlayerRepairing)
	{
		Timer += DeltaTime;
		if (Timer >= TimeToRepair)
		{
			Floor->RemoveHitFromArray(this);
			Destroy();
		}
	}
	else if (!IsPlayerRepairing && Timer != 0)
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
	ICouchInteractable::Interact_Implementation(Player);
	APlayer = Player;
	IsPlayerRepairing = !IsPlayerRepairing;
}




