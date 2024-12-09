#include "ShadowProjectile.h"

AShadowProjectile::AShadowProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void AShadowProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShadowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

