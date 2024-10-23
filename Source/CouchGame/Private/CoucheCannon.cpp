// Fill out your copyright notice in the Description page of Project Settings.


#include "CoucheCannon.h"

#include "CouchCannonBall.h"
#include "CouchWidget3D.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/OutputDeviceNull.h"


// Sets default values
ACoucheCannon::ACoucheCannon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACoucheCannon::BeginPlay()
{
	Super::BeginPlay();
	SetupTimeLine();
	StartPoint = this->FindComponentByTag<USceneComponent>(StartPointName);
	WidgetPose = this->FindComponentByTag<USceneComponent>(WidgetPoseName);
}

void ACoucheCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PowerTimeline.TickTimeline(DeltaTime);
}

#pragma region Spawn Functions

void ACoucheCannon::SpawnWidget(UClass* WidgetToSpawn)
{
	if (CurrentWidget == nullptr && WidgetPose != nullptr)
		CurrentWidget = GetWorld()->SpawnActor<AActor>(WidgetToSpawn, WidgetPose->GetComponentTransform());
	else
	{
		CurrentWidget->Destroy();
		CurrentWidget = GetWorld()->SpawnActor<AActor>(WidgetToSpawn, WidgetPose->GetComponentTransform());	
	}

	if (CurrentWidget->IsA<ACouchWidget3D>())
		PowerChargeActor = Cast<ACouchWidget3D>(CurrentWidget);
}

void ACoucheCannon::SpawnBullet()
{
	TargetLocation = LineTrace();
	FVector StartLocation = StartPoint->GetComponentLocation();
	FVector SuggestedVelocity;
	
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		SuggestedVelocity,
		StartLocation,
		TargetLocation,
		0,
		0.5
	);
	
	FTransform Transform = FTransform(SuggestedVelocity.Rotation(), StartPoint->GetComponentLocation());
	GetWorld()->SpawnActor<AActor>(Bullet, Transform);
	ACouchCannonBall* Projectile = GetWorld()->SpawnActor<ACouchCannonBall>(Bullet, Transform);
	if (Projectile)
	{
		Projectile->Initialize(SuggestedVelocity);
	}
}

#pragma endregion

#pragma region Charging

void ACoucheCannon::SetupTimeLine()
{
	PowerTimeline.SetPlayRate(SpeedCharge/1);
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("UpdatePower"));

	PowerTimeline.AddInterpFloat(PowerCurve, TimelineCallback);
	PowerTimeline.SetLooping(false);
}

void ACoucheCannon::StartCharging()
{
	PowerTimeline.PlayFromStart();
}

void ACoucheCannon::StopCharging()
{
	PowerTimeline.Stop();
	SpawnBullet();
}

void ACoucheCannon::UpdatePower(float Alpha)
{
    CurrentPower = Alpha * MaxPower;
	AttackRange = CurrentPower;

	FOutputDeviceNull ar;
	FString CmdAndParams = FString::Printf(TEXT("UpdatePower %f"), Alpha);
	PowerChargeActor->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);

	LineTrace();
}
#pragma endregion

FVector ACoucheCannon::LineTrace()
{
	const FVector Start = StartPoint->GetComponentLocation();
	const FVector End = Start + UKismetMathLibrary::GetForwardVector(StartPoint->GetComponentRotation()) * AttackRange;
       
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
      
	// Line Trace
	const bool bHit = UKismetSystemLibrary::LineTraceSingle(
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
