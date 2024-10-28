// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables//CoucheCannon.h"

#include "CouchCannonBall.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Characters/CouchCharacter.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/OutputDeviceNull.h"


// Sets default values
ACoucheCannon::ACoucheCannon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SetupCannon();
}

void ACoucheCannon::BeginPlay()
{
	Super::BeginPlay();
	
	SkeletalMesh = Cast<USkeletalMeshComponent>(this->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	
	SetupTimeLine();
}

void ACoucheCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PowerTimeline.TickTimeline(DeltaTime);
}

#pragma region Setup Functions

void ACoucheCannon::SetupCannon()
{
	WidgetComponent = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("SpawnerWidget"));
	MovementComponent = CreateDefaultSubobject<UCouchMovement>(TEXT("MovementComponent"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	WidgetPose = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetPose"));
	WidgetPose->SetupAttachment(RootComponent);
	PlayerPose = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerPose"));
	PlayerPose->SetupAttachment(RootComponent);
}

void ACoucheCannon::SetupTimeLine()
{
#pragma region Power Timeline
	PowerTimeline.SetPlayRate(1.f/SpeedCharge);
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("UpdatePower"));

	PowerTimeline.AddInterpFloat(PowerCurve, TimelineCallback);
	PowerTimeline.SetLooping(false);
#pragma endregion
}

#pragma endregion

void ACoucheCannon::Interact_Implementation(ACouchCharacter* Player)
{
	if (!CurrentPlayer || CurrentPlayer == Player)
	{
		if (!PlayerIsIn)
		{
			ACouchPickableCannonBall* Ball = Cast<ACouchPickableCannonBall>(GetWorld()->SpawnActor(ACouchPickableCannonBall::StaticClass()));
			Reload(Ball);
			CurrentPlayer = Player;
			PlayerIsIn = true;
			CanShoot = true;
			WidgetComponent->SpawnWidget(PowerChargeWidget, WidgetPose);
		
			FTransform PoseTransform = FTransform(PlayerPose->GetComponentRotation(), PlayerPose->GetComponentLocation());
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("PlayerPose Location: %s"), *PoseTransform.GetLocation().ToString()));

			Player->SetActorTransform(PoseTransform, false);
			Player->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			
		}
		else
		{
			PlayerIsIn = false;
			CanShoot = false;
			WidgetComponent->DestroyWidget();
			StopMovement();
			Player->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}	
	}
}

#pragma region Shoot

void ACoucheCannon::SpawnBullet()
{
	FVector StartLocation = SkeletalMesh->GetSocketLocation(FName("barrel"));
	FVector SuggestedVelocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		SuggestedVelocity,
		StartLocation,
		TargetLocation,
		0,
		0.5
	);

	DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Green, false, 3.0f, 0, 5.0f);
	
	FTransform Transform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
	ACouchCannonBall* Projectile = GetWorld()->SpawnActor<ACouchCannonBall>(Bullet, Transform);
	if (Projectile)
	{
		Projectile->Initialize(SuggestedVelocity);
		CanShoot = false;
		if (AmmoActor)
		{
			CurrentAmmo --;
			AmmoActor->Destroy();
			AmmoActor = nullptr;
		}
	}
}

FVector ACoucheCannon::LineTrace()
{
	const FVector Start = SkeletalMesh->GetSocketLocation(FName("barrel"));
	const FVector End = Start + UKismetMathLibrary::GetForwardVector(SkeletalMesh->GetSocketRotation(FName("barrel"))) * AttackRange;
       
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
      
	// Line Trace
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Blue,
		0.5f
	);
	return HitResult.TraceEnd;
}

#pragma region PowerCharging

void ACoucheCannon::StartCharging()
{
	if(CanShoot && CurrentAmmo >= 1)
	{
		if (WidgetComponent->PowerChargeActor)
		{
			PowerTimeline.PlayFromStart();
		}
		else
		{
			WidgetComponent->SpawnWidget(PowerChargeWidget, WidgetPose);
			StartCharging();	
		}
	}
}

void ACoucheCannon::StopCharging()
{
	if (CanShoot && CurrentAmmo >= 1)
	{
		PowerTimeline.Stop();
		if (SkeletalMesh) SkeletalMesh->PlayAnimation(ShootAnimation, false);
		FOutputDeviceNull ar;
		FString CmdAndParams = FString::Printf(TEXT("StopCharge %f"), SpeedCharge);
		if (WidgetComponent)
		{
			WidgetComponent->PowerChargeActor->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);
			WidgetComponent->DestroyWidget();
		}
	}
}

void ACoucheCannon::UpdatePower(float Alpha)
{
	if (WidgetComponent != nullptr)
	{
		CurrentPower = Alpha * MaxPower;
		AttackRange = CurrentPower;

		FOutputDeviceNull ar;
		FString CmdAndParams = FString::Printf(TEXT("UpdatePower %f"), SpeedCharge);
		WidgetComponent->PowerChargeActor->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);

		TargetLocation = LineTrace();	
	}
}

#pragma endregion

#pragma endregion

#pragma region Reload

void ACoucheCannon::Reload(ACouchPickableCannonBall* CannonBallReload)
{
	if(CurrentAmmo < 1)
	{
		AmmoActor = CannonBallReload;
		CannonBallReload->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("barrel"));
		CurrentAmmo = 1;
		CanShoot = true;		
	}
}

#pragma endregion

#pragma region Movement

void ACoucheCannon::StartMovement(int InputDirection)
{
	if (PlayerIsIn)
	{
		CanShoot = false;
		MovementComponent->StartMovement(InputDirection);
	}
}

void ACoucheCannon::StopMovement()
{
	if (PlayerIsIn)
	{
		CanShoot = true;
		MovementComponent->StopMovement();	
	}
}

#pragma endregion
