// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Fog/Fog.h"


// Sets default values
AFog::AFog()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;
}

// Called when the game starts or when spawned
void AFog::BeginPlay()
{
	Super::BeginPlay();
	// Vérifie si le mesh existe
	if (StaticMeshComponent)
	{
		if (UMaterialInterface* BaseMaterial = StaticMeshComponent->GetMaterial(0))
		{
			MaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		
			StaticMeshComponent->SetMaterial(0, MaterialInstance);
		}
	}
}

// Called every frame
void AFog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFog::UpdateFog(float Value) const
{
	if (MaterialInstance)
	{
		MaterialInstance->SetScalarParameterValue(FName("Opacity"), FMath::Clamp(Value, 0, 1));
	}
}

