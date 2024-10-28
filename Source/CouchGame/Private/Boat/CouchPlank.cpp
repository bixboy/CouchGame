// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat/CouchPlank.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "Components/SceneComponent.h"
#include "Widget/CouchWidgetSpawn.h"
#include "Characters/CouchCharacter.h"


// Sets default values
ACouchPlank::ACouchPlank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlankMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlankMesh");
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	WaterParticle = CreateDefaultSubobject<UNiagaraComponent>("Water");
	WidgetPos = CreateDefaultSubobject<USceneComponent>("WidgetPos");
	CouchWidgetSpawn = CreateDefaultSubobject<UCouchWidgetSpawn>("CouchWidgetSpawn");
	
	RootComponent = PlankMesh;
	UStaticMesh* RandomStaticMesh = GetRandomStaticMesh();
	if (RandomStaticMesh) PlankMesh->SetStaticMesh(GetRandomStaticMesh()); // Je mets un mesh aléatoire
	BoxComponent->SetupAttachment(PlankMesh);
	WidgetPos->SetupAttachment(PlankMesh);
	WaterParticle->SetupAttachment(PlankMesh);


	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACouchPlank::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACouchPlank::OnBoxEndOverlap);
}

ACouchPlank::~ACouchPlank()
{
	BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ACouchPlank::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.RemoveDynamic(this, &ACouchPlank::OnBoxEndOverlap);
}

// Called when the game starts or when spawned
void ACouchPlank::BeginPlay()
{
	Super::BeginPlay();
}

UStaticMesh* ACouchPlank::GetRandomStaticMesh()
{
	if (DamagedPlanckMeshes.Num() == 0) return nullptr;
	int RandomIndex = FMath::RandRange(0, DamagedPlanckMeshes.Num() - 1);
	return DamagedPlanckMeshes[RandomIndex];
}

void ACouchPlank::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACouchCharacter::StaticClass()))
	{
		IsInInterractZone = true;
		CouchWidgetSpawn->SpawnWidget(InterractWidget, WidgetPos);
	}
	
}

void ACouchPlank::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ACouchCharacter::StaticClass()))
	{
		IsInInterractZone = false;
		CouchWidgetSpawn->DestroyWidget();
	}
}



