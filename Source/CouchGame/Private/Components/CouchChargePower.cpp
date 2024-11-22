#include "Components//CouchChargePower.h"

#include "Characters/CouchCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/OutputDeviceNull.h"

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


void UCouchChargePower::StartCharging(USkeletalMeshComponent* MeshComp, UCouchWidgetSpawn* WidgetSpawner, bool UseMesh,  ACouchCharacter* Actor)
{
	PowerTimeline.PlayFromStart();
	if (UseMesh)
	{
		Mesh = MeshComp;	
	}
	else if (!UseMesh)
	{
		Mesh = MeshComp;
		StartTransform = FTransform(FRotator(Actor->GetActorRotation().Pitch, Actor->GetMesh()->GetComponentRotation().Yaw + 90.f, Actor->GetActorRotation().Roll), FVector(Actor->GetActorLocation()), FVector(1, 1, 1));
	}
	UsedMesh = UseMesh;
	ChargeWidget = WidgetSpawner;
}


void UCouchChargePower::StopCharging()
{
	PowerTimeline.Stop();
	if (ChargeWidget && ChargeWidget->PowerChargeActor)
	{
		FOutputDeviceNull ar;
		FString CmdAndParams = FString::Printf(TEXT("StopCharge"));
			
		ChargeWidget->PowerChargeActor->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);
		ChargeWidget->DestroyWidget();
	}
}


float UCouchChargePower::UpdatePower(float Alpha)
{
	CurrentPower = Alpha * MaxPower;
	TargetLocation = LineTrace();
	if (ChargeWidget && ChargeWidget->PowerChargeActor)
	{
		FOutputDeviceNull ar;
		FString CmdAndParams = FString::Printf(TEXT("UpdatePower %f"), SpeedCharge);
		ChargeWidget->PowerChargeActor->CallFunctionByNameWithArguments(*CmdAndParams, ar, NULL, true);
	}
	return CurrentPower;
}


FVector UCouchChargePower::LineTrace()
{
	if (!Mesh)
	{
		return FVector::ZeroVector;
	}

	if (UsedMesh)
	{
		Start = Mesh->GetSocketLocation(FName("barrel"));
		End = Start + UKismetMathLibrary::GetForwardVector(Mesh->GetSocketRotation(FName("barrel"))) * CurrentPower;	
	}
	else if (!UsedMesh)
	{
		Start = StartTransform.GetLocation();
		End = Start + UKismetMathLibrary::GetForwardVector(StartTransform.Rotator()) * CurrentPower;
	}
     
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	FHitResult HitResult;
    
	// Line Trace*
	UKismetSystemLibrary::LineTraceSingle(
	   GetWorld(),
	   Start,
	   End,
	   UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1),
	   false,
	   ActorsToIgnore,
	   EDrawDebugTrace::None,
	   HitResult,
	   true,
	   FLinearColor::Red,
	   FLinearColor::Blue,
	   0.5f
	);
	return HitResult.TraceEnd;
}

