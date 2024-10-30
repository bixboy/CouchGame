#include "Components//CouchChargePower.h"


#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


UCouchChargePower::UCouchChargePower()
{
	PrimaryComponentTick.bCanEverTick = true;
}




void UCouchChargePower::BeginPlay()
{
	Super::BeginPlay();
	PowerTimeline.SetPlayRate(1.f/SpeedCharge);
	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("UpdatePower"));


	PowerTimeline.AddInterpFloat(PowerCurve, TimelineCallback);
	PowerTimeline.SetLooping(false);
}


void UCouchChargePower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PowerTimeline.TickTimeline(DeltaTime);
}


void UCouchChargePower::StartCharging(USkeletalMeshComponent* MeshComp)
{
	PowerTimeline.PlayFromStart();
	Mesh = MeshComp;
}


void UCouchChargePower::StopCharging()
{
	PowerTimeline.Stop();
}


float UCouchChargePower::UpdatePower(float Alpha)
{
	CurrentPower = Alpha * MaxPower;
	TargetLocation = LineTrace();
	return CurrentPower;
}


FVector UCouchChargePower::LineTrace()
{
	if (!Mesh)
	{
		return FVector::ZeroVector;
	}
  
	const FVector Start = Mesh->GetSocketLocation(FName("barrel"));
	const FVector End = Start + UKismetMathLibrary::GetForwardVector(Mesh->GetSocketRotation(FName("barrel"))) * CurrentPower;
     
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
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

