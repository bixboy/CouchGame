#include "CouchLure.h"

#include "Components/CouchProjectile.h"
#include "Interfaces/CouchInteractable.h"


ACouchLure::ACouchLure()
{
	PrimaryActorTick.bCanEverTick = true;
	CouchProjectile = CreateDefaultSubobject<UCouchProjectile>(TEXT("ProjectileComponent"));
  
	SphereComponent =  CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	LureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LureMesh"));
	LureMesh->SetupAttachment(RootComponent);
	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TopMesh"));
	TopMesh->SetupAttachment(RootComponent);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACouchLure::OnLureBeginOverlap);
}

void ACouchLure::OnLureBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CouchProjectile->CanMove)
	{
		LastLocation = GetActorLocation();
		SphereComponent->SetSimulatePhysics(false);
		CouchProjectile->CanMove = false;
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACouchLure::SetPhysics, 0.5f, false);
	}
	
	if (OtherActor->Implements<UCouchInteractable>())
	{
		//AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ACouchLure::SetPhysics()
{
	SphereComponent->SetSimulatePhysics(true);
	SetActorLocation(LastLocation);
}


