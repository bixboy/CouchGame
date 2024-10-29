// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CouchWidgetSpawn.h"
#include "Widget/CouchWidget3D.h"


UCouchWidgetSpawn::UCouchWidgetSpawn()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCouchWidgetSpawn::BeginPlay()
{
	Super::BeginPlay();
}

AActor* UCouchWidgetSpawn::GetCurrentWidget()
{
	return CurrentWidget;
}

void UCouchWidgetSpawn::SpawnWidget(UClass* WidgetToSpawn, USceneComponent* AttachParent)
{
	if (CurrentWidget == nullptr && AttachParent != nullptr)
	{
		CurrentWidget = GetWorld()->SpawnActor<AActor>(WidgetToSpawn, AttachParent->GetComponentTransform());
	}
	else if (CurrentWidget != nullptr)
	{
		CurrentWidget->Destroy();
		CurrentWidget = GetWorld()->SpawnActor<AActor>(WidgetToSpawn, AttachParent->GetComponentTransform());
	}

	// Vérification de CurrentWidget avant l’attachement
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->AttachToComponent(AttachParent, FAttachmentTransformRules::KeepWorldTransform);

		if (CurrentWidget->IsA<ACouchWidget3D>())
		{
			PowerChargeActor = Cast<ACouchWidget3D>(CurrentWidget);
			
		}
	}
}

void UCouchWidgetSpawn::DestroyWidget()
{
	if (CurrentWidget != nullptr)
	{
		if (CurrentWidget->IsA<ACouchWidget3D>())
			PowerChargeActor = nullptr;
		
		CurrentWidget->Destroy();
		CurrentWidget = nullptr;
	}
}

void UCouchWidgetSpawn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

