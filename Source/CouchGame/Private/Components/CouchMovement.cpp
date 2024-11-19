// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CouchMovement.h"


UCouchMovement::UCouchMovement()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCouchMovement::BeginPlay()
{
	Super::BeginPlay();
	if (LinePath)
		LinePathComponent = Cast<USplineComponent>(LinePath->GetComponentByClass(USplineComponent::StaticClass()));

#pragma region Move Timeline
	if(LinePathComponent)
	{
		MoveTimeline.SetPlayRate(SpeedMovement/1.f);
		FOnTimelineFloat MoveTimelineCallback;
		MoveTimelineCallback.BindUFunction(this, FName("MoveActor"));

		MoveTimeline.AddInterpFloat(MoveCurve, MoveTimelineCallback);
		MoveTimeline.SetLooping(false);	
	}
#pragma endregion
}

void UCouchMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MoveTimeline.TickTimeline(DeltaTime);
}



void UCouchMovement::StartMovement(int InputDirection)
{
	int Direction = FMath::Clamp(InputDirection, -1, 1);
	
	if (Direction == 1)
		MoveTimeline.Play();
	else
		MoveTimeline.Reverse();	
}

void UCouchMovement::StopMovement()
{
		MoveTimeline.Stop();	
}

void UCouchMovement::MoveActor(float Alpha)
{
	if (LinePathComponent)
	{
		float Distance = FMath::Lerp(0, LinePathComponent->GetSplineLength(), Alpha);
		FVector Location = LinePathComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator Rotation =	FRotator(GetOwner()->GetActorRotation()); //LinePathComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
		FTransform Transform = FTransform(Rotation, Location, GetOwner()->GetActorScale());
		GetOwner()->SetActorTransform(Transform, false);
	}
}


bool UCouchMovement::GetCanMove() const
{
	return CanMove;
}

void UCouchMovement::SetCanMove(bool Value)
{
	CanMove = Value;
}

