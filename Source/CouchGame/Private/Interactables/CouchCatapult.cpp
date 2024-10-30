// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactables/CouchCatapult.h"

#include "CouchCannonBall.h"
#include "Components/CouchChargePower.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"

ACouchCatapult::ACouchCatapult()
{
	PrimaryActorTick.bCanEverTick = true;
	PowerChargeComponent = CreateDefaultSubobject<UCouchChargePower>(TEXT("PowerCharge"));
}

void ACouchCatapult::Interact_Implementation(ACouchCharacter* Player)
{
	Super::Interact_Implementation(Player);
	
	if (!GetCurrentPlayer() || GetCurrentPlayer() == Player)
	{
		if (!Execute_IsUsedByPlayer(this))
		{
			ACouchPickableCannonBall* Ball = Cast<ACouchPickableCannonBall>(GetWorld()->SpawnActor(ACouchPickableCannonBall::StaticClass()));
			Reload(Ball);
		}
	}	
}

#pragma region Charging Component

void ACouchCatapult::StartChargeActor_Implementation()
{
	ICouchInteractable::StartChargeActor_Implementation();
	if(GetCanUse() && CurrentAmmo >= 1 && !IsInCharge)
	{
		if (WidgetComponent->PowerChargeActor)
		{
			PowerChargeComponent->StartCharging(SkeletalMesh);
			IsInCharge = true;
		}
		else
		{
			WidgetComponent->SpawnWidget(PowerChargeWidget, WidgetPose);
			ICouchInteractable::Execute_StartChargeActor(this);
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
		
		if (WidgetComponent && WidgetComponent->PowerChargeActor)
		{
			FOutputDeviceNull ar;
			FString CmdAndParams = FString::Printf(TEXT("StopCharge"));
			
			WidgetComponent->PowerChargeActor->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);
			WidgetComponent->DestroyWidget();
		}
		
		SetCanUse(false);
		IsInCharge = false;
	}
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

	// DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Green, false, 3.0f, 0, 5.0f);
	
	FTransform Transform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
	ACouchCannonBall* Projectile = GetWorld()->SpawnActor<ACouchCannonBall>(Bullet, Transform);
	if (Projectile)
	{
		Projectile->Initialize(SuggestedVelocity);
		if (AmmoActor)
		{
			CurrentAmmo --;
			AmmoActor->Destroy();
			AmmoActor = nullptr;
		}
	}
}

void ACouchCatapult::Reload(ACouchPickableCannonBall* CannonBallReload)
{
	if(CurrentAmmo < 1 && !GetCanUse())
	{
		AmmoActor = CannonBallReload;
		CannonBallReload->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("barrel"));
		CurrentAmmo = 1;
		SetCanUse(true);		
	}
}

#pragma endregion

