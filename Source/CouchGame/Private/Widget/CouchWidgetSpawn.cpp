// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/CouchWidgetSpawn.h"
#include "Widget/CouchWidget3D.h"


UCouchWidgetSpawn::UCouchWidgetSpawn()
{
	PrimaryComponentTick.bCanEverTick = true;
}

ACouchWidget3D* UCouchWidgetSpawn::GetCurrentWidget()
{
	return CurrentWidget;
}

void UCouchWidgetSpawn::SpawnWidget(TSubclassOf<ACouchWidget3D> WidgetToSpawn, USceneComponent* AttachParent, bool WantToAttach)
{
	if (CurrentWidget == nullptr)
	{
		if (AttachParent)
			CurrentWidget = GetWorld()->SpawnActor<ACouchWidget3D>(WidgetToSpawn, AttachParent->GetComponentTransform());
	}
	else if (CurrentWidget != nullptr)
	{
		CurrentWidget->Destroy();
		
		if (AttachParent)
			CurrentWidget = GetWorld()->SpawnActor<ACouchWidget3D>(WidgetToSpawn, AttachParent->GetComponentTransform());
	}

	// Vérification de CurrentWidget avant l’attachement
	if (CurrentWidget != nullptr)
	{
		if (AttachParent && WantToAttach)
			CurrentWidget->AttachToComponent(AttachParent, FAttachmentTransformRules::KeepWorldTransform);

		if (CurrentWidget->IsA<ACouchWidget3D>())
		{
			PowerChargeActor = Cast<ACouchWidget3D>(CurrentWidget);
			
		}
	}
}

void UCouchWidgetSpawn::DestroyWidget()
{
	if (CurrentWidget)
	{
		if (CurrentWidget->IsA<ACouchWidget3D>())
		{
			PowerChargeActor = nullptr;
		}
		CurrentWidget->Destroy();
		CurrentWidget = nullptr;
	}
}

