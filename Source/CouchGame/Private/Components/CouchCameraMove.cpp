#include "Components/CouchCameraMove.h"
#include "Kismet/KismetMathLibrary.h"

UCouchCameraMove::UCouchCameraMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCouchCameraMove::BeginPlay()
{
	Super::BeginPlay();
  
	MoveTimeline.SetPlayRate(1.f/Speed);
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("MoveCamera"));


	MoveTimeline.AddInterpFloat(MoveCurve, TimelineCallback);
	MoveTimeline.SetLooping(false);


	FOnTimelineEvent OnTimelineFinishEvent;
	OnTimelineFinishEvent.BindUFunction(this, FName("EndCameraMove"));
	MoveTimeline.SetTimelineFinishedFunc(OnTimelineFinishEvent);
	PointA = GetOwner()->GetActorLocation();
	// StartCameraMove();
}


void UCouchCameraMove::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MoveTimeline.TickTimeline(DeltaTime);
}

void UCouchCameraMove::StartCameraMove(bool Forward)
{
	bIsMoving = true;
	isPlayingForward = Forward;
	if (Forward) MoveTimeline.Play();
	else MoveTimeline.Reverse();
}

void UCouchCameraMove::MoveCamera(float Alpha)
{
	if (bIsMoving && Boat2 && Boat1)
	{
		// Interpolation entre PointA et DestinationPoint basée sur la progression de la Timeline
		FVector NewLocation = FMath::Lerp(PointA, DestinationPoint, Alpha);

		// Ajoute l'effet de plongée (Plunge)
		float PlungeOffset = FMath::Sin(Alpha * PI) * PlungeHeight;
		NewLocation.Z += PlungeOffset;

		// Met à jour la position de la caméra
		GetOwner()->SetActorLocation(NewLocation);

		// Regarde vers le point médian entre les deux bateaux
		FVector MidPoint = (Boat1->GetActorLocation() + Boat2->GetActorLocation()) * 0.5f;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, MidPoint);

		// Rendre la rotation plus rapide près de la fin
		float DynamicSpeed = FMath::Lerp(1.f, 1.f * 10.0f, Alpha);

		// Interpolation fluide de la rotation
		FRotator CurrentRotation = GetOwner()->GetActorRotation();
		FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), DynamicSpeed);
		GetOwner()->SetActorRotation(SmoothRotation);
	}
}

void UCouchCameraMove::ReversMoveCamera()
{
	bIsMoving = true;
	MoveTimeline.Reverse();
}

void UCouchCameraMove::EndCameraMove()
{
	if (isPlayingForward) TravelingEnd.Broadcast(isPlayingForward);
}


