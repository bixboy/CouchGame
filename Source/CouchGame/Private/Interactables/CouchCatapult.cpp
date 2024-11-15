// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchCatapult.h"
#include "CouchCannonBall.h"
#include "CouchStaticCanonBall.h"
#include "Components/CouchChargePower.h"
#include "Kismet/GameplayStatics.h"

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
		if (WidgetComponent->PowerChargeActor)
		{
			PowerChargeComponent->StartCharging(SkeletalMesh, WidgetComponent);
			IsInCharge = true;
		}
		else
		{
			WidgetComponent->SpawnWidget(PowerChargeWidget, WidgetPose);
			PowerChargeComponent->StartCharging(SkeletalMesh, WidgetComponent);
			IsInCharge = true;
		}
	}
}

void ACouchCatapult::StopChargeActor_Implementation()
{
	ICouchInteractable::StopChargeActor_Implementation();
	if (GetCanUse() && CurrentAmmo >= 1 && IsInCharge)
	{
		PowerChargeComponent->StopCharging();
		if (SkeletalMesh && ShootAnimation) SkeletalMesh->PlayAnimation(ShootAnimation, false);
		
		SetCanUse(false);
		IsInCharge = false;
	}
}

void ACouchCatapult::Interact_Implementation(ACouchCharacter* Player)
{
	if (IsInCharge && Execute_IsUsedByPlayer(this))
	{
		IsInCharge = false;
	}
	Super::Interact_Implementation(Player);
}

#pragma endregion


#pragma region Shoot / Reload

void ACouchCatapult::SpawnBullet()
{
	FVector StartLocation = SkeletalMesh->GetSocketLocation(FName("barrel"));
	FVector SuggestedVelocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		SuggestedVelocity,
		StartLocation,
		PowerChargeComponent->TargetLocation,
		0,
		CurveShoot
	);
	
	FTransform Transform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
	ACouchCannonBall* Projectile = GetWorld()->SpawnActor<ACouchCannonBall>(Bullet, Transform);
	if (Projectile)
	{
		Projectile->InitCanonBall(AmmoActor);
		Projectile->Initialize(SuggestedVelocity);
		if (AmmoActor)
		{
			CurrentAmmo --;
			AmmoActor->Destroy();
			AmmoActor = nullptr;
		}
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShake, 1.0f);
	}
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

