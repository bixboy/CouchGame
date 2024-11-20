#include "Components/CouchCameraMove.h"
#include "Kismet/KismetMathLibrary.h"

UCouchCameraMove::UCouchCameraMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCouchCameraMove::BeginPlay()
{
	Super::BeginPlay();
	StartCameraMove();
}

void UCouchCameraMove::StartCameraMove()
{
	PointA = GetOwner()->GetActorLocation();
	bIsMoving = true;
}

void UCouchCameraMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsMoving)
	{
		MoveCamera(DeltaTime);	
	}
}

void UCouchCameraMove::MoveCamera(float DeltaTime)
{
	TimeElapsed += DeltaTime;
	float Alpha = FMath::Clamp(TimeElapsed / Duration, 0.0f, 1.0f);

	// Calculate position
	FVector NewLocation = FMath::Lerp(PointA, DestinationPoint, Alpha);

	// Add plunging effect
	float PlungeOffset = FMath::Sin(Alpha * PI) * PlungeHeight;
	NewLocation.Z += PlungeOffset;

	// Set camera location
	GetOwner()->SetActorLocation(NewLocation);

	// Look at the midpoint between boats
	FVector MidPoint = (Boat1->GetActorLocation() + Boat2->GetActorLocation()) * 0.5f;
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(NewLocation, MidPoint);
	GetOwner()->SetActorRotation(LookAtRotation);

	if (Alpha >= 1.0f)
	{
		bIsMoving = false;
	}
}

