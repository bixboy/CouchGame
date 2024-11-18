// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Storm/CouchProjectileEffectStorm.h"

#include "CouchCannonBall.h"
#include "Boat/CouchPlank.h"
#include "ProjectileEffect/Effects/Storm/Storm.h"


// Sets default values
ACouchProjectileEffectStorm::ACouchProjectileEffectStorm()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ACouchProjectileEffectStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasExecutedEffect && StormPtr)
	{
		Timer += DeltaTime;
		if (Timer >= DelayToExecute)
		{
			StormPtr->Destroy();
			Destroy();
		}
	}
}



void ACouchProjectileEffectStorm::ExecuteEffect()
{
	Super::ExecuteEffect();
	if (!CouchCannonBall) return;
	FTransform Transform = FTransform(FRotator::ZeroRotator, CouchCannonBall->GetActorLocation(), FVector(1.0f));
	StormPtr = GetWorld()->SpawnActor<AStorm>(Storm, Transform);
	if (StormPtr) StormPtr->StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACouchProjectileEffectStorm::OnActorOverlapStorm);
	CouchCannonBall->Destroy();
}

void ACouchProjectileEffectStorm::OnActorOverlapStorm(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	StormEffect(OtherActor);
}


void ACouchProjectileEffectStorm::StormEffect(AActor* Actor) const
{
	if (!Actor) return;

	// Vérifie si l'Actor est un Character
	ACharacter* Character = Cast<ACharacter>(Actor);
	if (Character)
	{
		// Calculer une direction aléatoire
		FVector RandomDirection = FMath::VRand();
		RandomDirection.Z = FMath::FRandRange(0.5f, 1.0f); // Ajoute une poussée verticale
		RandomDirection.Normalize();

		// Calculer la force en fonction de StormStrength
		FVector LaunchVelocity = RandomDirection * StormStrength;

		// Appliquer la poussée au personnage
		Character->LaunchCharacter(LaunchVelocity, true, true);
		UE_LOG(LogTemp, Log, TEXT("Tempête : Lancement du Character %s avec une vélocité : %s"), *Character->GetName(), *LaunchVelocity.ToString());
		return; // Terminé pour un personnage
	}

	
	if (!Actor->IsA(ACouchPlank::StaticClass()))
	{
		UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (RootComp)
		{
			FVector RandomDirection = FMath::VRand();
			RandomDirection.Z = FMath::FRandRange(0.2f, 1.0f);
			RandomDirection.Normalize();

			FVector Impulse = RandomDirection * StormStrength;
			RootComp->AddImpulse(Impulse, NAME_None, true);
			UE_LOG(LogTemp, Log, TEXT("Impulse appliquée à %s : %s"), *Actor->GetName(), *Impulse.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("L'Actor %s n'a pas de composant physique valide pour une impulsion."), *Actor->GetName());
		}
	}

}



