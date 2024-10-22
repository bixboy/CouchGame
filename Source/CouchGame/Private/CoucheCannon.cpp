// Fill out your copyright notice in the Description page of Project Settings.


#include "CoucheCannon.h"
#include "CouchCannonBall.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


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
}

void ACoucheCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PowerTimeline.TickTimeline(DeltaTime);
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
		CurrentPower
	);
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("Power: %f"), CurrentPower));
	FTransform Transform = FTransform(SuggestedVelocity.Rotation(), StartPoint->GetComponentLocation());
	GetWorld()->SpawnActor<AActor>(Bullet, Transform);
}

#pragma region Charging

void ACoucheCannon::SetupTimeLine()
{
	PowerTimeline.SetPlayRate(SpeedInterp/1);
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
		EDrawDebugTrace::Persistent,
		HitResult,
		true,
		FLinearColor::Gray,
		FLinearColor::Blue,
		0.5f
	);

	if (bHit)
	{
		return HitResult.ImpactPoint;
	}
	else
	{
		return HitResult.TraceEnd;
	}
}
