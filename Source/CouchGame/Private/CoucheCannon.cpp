// Fill out your copyright notice in the Description page of Project Settings.


#include "CoucheCannon.h"
#include "CouchCannonBall.h"
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
	StartPoint = this->FindComponentByTag<USceneComponent>(StartPointName);
	Shoot();
}

void ACoucheCannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoucheCannon::Shoot()
{
	FTransform Transform = FTransform(	LineTrace(), StartPoint->GetComponentLocation());
	GetWorld()->SpawnActor<AActor>(Bullet, Transform);
}

FRotator ACoucheCannon::LineTrace()
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
		return UKismetMathLibrary::FindLookAtRotation(StartPoint->GetComponentLocation(),HitResult.ImpactPoint);
	}
	else
	{
		return UKismetMathLibrary::FindLookAtRotation(StartPoint->GetComponentLocation(),HitResult.TraceEnd);
	}
	
}
