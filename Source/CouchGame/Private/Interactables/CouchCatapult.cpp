// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchCatapult.h"
#include "CouchCannonBall.h"
#include "CouchStaticCanonBall.h"
#include "Components/CouchChargePower.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectileEffect/CouchProjectileEffect.h"
#include "ProjectileEffect/CouchProjectileExecuteTime.h"
#include "ProjectileEffect/Effects/CouchProjectileEffectTripleBullets.h"

ACouchCatapult::ACouchCatapult()
{
	PrimaryActorTick.bCanEverTick = true;
	PowerChargeComponent = CreateDefaultSubobject<UCouchChargePower>(TEXT("PowerCharge"));
}

#pragma region Charging Component

void ACouchCatapult::StartChargeActor_Implementation()
{
	ICouchInteractable::StartChargeActor_Implementation();

	if(GetCanUse() && CurrentAmmo >= 1 && !IsInCharge)
	{
		PlayFx();
		if (WidgetComponent->PowerChargeActor)
		{
			PowerChargeComponent->StartCharging(SkeletalMesh, WidgetComponent, true);
			if (CurrentPlayer) CurrentPlayer->AnimationManager->IsChargingCatapult = true;
			IsInCharge = true;
			MovementComponent->SetCanMove(true);
		}
		else
		{
			WidgetComponent->SpawnWidget(PowerChargeWidget, WidgetPose);
			PowerChargeComponent->StartCharging(SkeletalMesh, WidgetComponent, true);
			if (CurrentPlayer) CurrentPlayer->AnimationManager->IsChargingCatapult = true;
			IsInCharge = true;
			MovementComponent->SetCanMove(true);
		}
	}
}

void ACouchCatapult::StopChargeActor_Implementation()
{
	ICouchInteractable::StopChargeActor_Implementation();
	if (GetCanUse() && CurrentAmmo >= 1 && IsInCharge)
	{
		PowerChargeComponent->StopCharging();
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsChargingCatapult = false;
		if (SkeletalMesh && ShootAnimation) SkeletalMesh->PlayAnimation(ShootAnimation, false);
		if (CurrentPlayer) CurrentPlayer->AnimationManager->IsReleasingCatapult = true;
		
		SetCanUse(false);
		IsInCharge = false;
		MovementComponent->SetCanMove(true);
	}
}

void ACouchCatapult::Interact_Implementation(ACouchCharacter* Player)
{
	if (IsInCharge && Execute_IsUsedByPlayer(this))
	{
		Execute_StopChargeActor(this);
	}
	Super::Interact_Implementation(Player);
}

#pragma endregion


#pragma region Shoot / Reload

void ACouchCatapult::SpawnBullet()
{
    FVector StartLocation = SkeletalMesh->GetSocketLocation(FName("barrel"));
    FVector SuggestedVelocity;
    
    // Calculer la vélocité suggérée pour le projectile principal
    UGameplayStatics::SuggestProjectileVelocity_CustomArc(
        this,
        SuggestedVelocity,
        StartLocation,
        PowerChargeComponent->TargetLocation,
        0,
        CurveShoot
    );
    
    FTransform Transform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
    ACouchCannonBall* MainProjectile = GetWorld()->SpawnActor<ACouchCannonBall>(Bullet, Transform);
    
    if (MainProjectile)
    {
        MainProjectile->InitCanonBall(AmmoActor);
        MainProjectile->Initialize(SuggestedVelocity);
    }
    
    if (ACouchProjectileEffect* ProjectileEffect = MainProjectile->GetEffectWithExecuteTime(ECouchProjectileExecuteTime::OnLaunch))
    {
        if (ACouchProjectileEffectTripleBullets* TripleBulletsEffect = Cast<ACouchProjectileEffectTripleBullets>(ProjectileEffect))
        {
            float AngleOffset = TripleBulletsEffect->Degree;

            // Calculer la vélocité pour les projectiles supplémentaires (gauche et droite)
            FVector LeftVelocity;
    
        	// Calculer la vélocité suggérée pour le projectile principal
        	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
				this,
				LeftVelocity,
				StartLocation,
				PowerChargeComponent->TargetLocation + FVector(-AngleOffset * UE::Geometry::Distance(StartLocation, PowerChargeComponent->TargetLocation), 0, 0),
				0,
				CurveShoot
			);
            FVector RightVelocity = SuggestedVelocity;
        	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
				this,
				RightVelocity,
				StartLocation,
				PowerChargeComponent->TargetLocation + FVector(AngleOffset * UE::Geometry::Distance(StartLocation, PowerChargeComponent->TargetLocation), 0, 0),
				0,
				CurveShoot
			);
            // Appliquer la rotation spécifique pour chaque projectile
            FRotator LeftRotation = LeftVelocity.Rotation().Add(0, 0, -AngleOffset);
            FRotator RightRotation = RightVelocity.Rotation().Add(0, 0, AngleOffset);
        	
            // Spawner les projectiles gauche et droit avec leurs nouvelles vélocités
            FTransform LeftTransform = FTransform(LeftRotation, SkeletalMesh->GetSocketLocation(FName("barrel")));
            FTransform RightTransform = FTransform(RightRotation, SkeletalMesh->GetSocketLocation(FName("barrel")));

            ACouchCannonBall* LeftProjectile = GetWorld()->SpawnActor<ACouchCannonBall>(Bullet, LeftTransform);
            ACouchCannonBall* RightProjectile = GetWorld()->SpawnActor<ACouchCannonBall>(Bullet, RightTransform);
			AmmoActor->ProjectileEffects.Remove(ProjectileEffect->GetClass());
            if (LeftProjectile)
            {
                LeftProjectile->InitCanonBall(AmmoActor);
                LeftProjectile->Initialize(LeftVelocity); // Utilise la vélocité recalculée
            }
            if (RightProjectile)
            {
                RightProjectile->InitCanonBall(AmmoActor);
                RightProjectile->Initialize(RightVelocity); // Utilise la vélocité recalculée
            }
        }
    }

    if (AmmoActor)
    {
        CurrentAmmo--;
        AmmoActor->Destroy();
        AmmoActor = nullptr;
        if (CurrentPlayer)
        {
        	CurrentPlayer->AnimationManager->IsChargingCatapult= false;
	        CurrentPlayer->AnimationManager->IsReleasingCatapult = false;
        }
    }
    GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake, 1.0f);
}


void ACouchCatapult::Reload(ACouchPickableCannonBall* CannonBallReload)
{
	if(CurrentAmmo < 1)
	{
		FTransform Transform = FTransform(FRotator::ZeroRotator,
			SkeletalMesh->GetSocketLocation(FName("barrel")),
			CannonBallReload->GetActorScale());
		ACouchStaticCanonBall* StaticBall = GetWorld()->SpawnActor<ACouchStaticCanonBall>(StaticBullet,Transform);
		CannonBallReload->Destroy();
		StaticBall->InitCanonBall(CannonBallReload);
		StaticBall->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("barrel"));

		AmmoActor = StaticBall;
		CurrentAmmo = 1;
		SetCanUse(true);		
	}
}

#pragma endregion

