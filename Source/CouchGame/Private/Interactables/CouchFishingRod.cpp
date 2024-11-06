#include "Interactables/CouchFishingRod.h"
#include "CouchLure.h"
#include "Components/CouchChargePower.h"
#include "Interfaces/CouchInteractable.h"
#include "Kismet/GameplayStatics.h"


ACouchFishingRod::ACouchFishingRod()
{
   PrimaryActorTick.bCanEverTick = false;
   ChargePower = CreateDefaultSubobject<UCouchChargePower>(TEXT("ChargePower"));


   SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
   RootComponent = SkeletalMesh;
}

bool ACouchFishingRod::IsUsedByPlayer_Implementation() {return ICouchInteractable::IsUsedByPlayer_Implementation();}

#pragma region ChargingPower

void ACouchFishingRod::StartChargeActor_Implementation()
{
   ICouchInteractable::StartChargeActor_Implementation();
   ChargePower->StartCharging(SkeletalMesh);
}

void ACouchFishingRod::StopChargeActor_Implementation()
{
   ICouchInteractable::StopChargeActor_Implementation();
   ChargePower->StopCharging();
   SpawnLure();
}

#pragma endregion 

#pragma region Spawn & Init

void ACouchFishingRod::SpawnLure()
{
   FVector StartLocation = SkeletalMesh->GetSocketLocation(FName("barrel"));
   FVector SuggestedVelocity;
  
   UGameplayStatics::SuggestProjectileVelocity_CustomArc(
      this,
      SuggestedVelocity,
      StartLocation,
      ChargePower->TargetLocation,
      0,
      0.5
   );
  
   FTransform SpawnTransform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
   if (LureRef = GetWorld()->SpawnActor<ACouchLure>(Lure, SpawnTransform))
   {
      LureRef->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
      InitializeCableAndConstraint();
      LureRef->Initialize(SuggestedVelocity, this);  
   }
}

void ACouchFishingRod::InitializeCableAndConstraint()
{
   // Ajoute le composant Cable
   Cable = NewObject<UCableComponent>(this, TEXT("Cable"));
   if (Cable)
   {
      Cable->SetupAttachment(RootComponent);
      Cable->RegisterComponent();

      // Setup Cable
      Cable->SetRelativeScale3D(FVector(CableScale, CableScale, CableScale));
      Cable->SetMaterial(0, CableMaterial);
      Cable->NumSegments = 17.f;
      float Distance = FVector::Dist(SkeletalMesh->GetSocketLocation(FName("barrel")), ChargePower->TargetLocation);
      Cable->CableLength = Distance;

      // Attache le cable
      Cable->SetAttachEndToComponent(LureRef->SphereComponent);
      Cable->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("barrel"));
      Cable->SetAttachEndTo(LureRef, FName(TEXT("TopMesh")), FName(TEXT("barrel")));

      Cable->EndLocation = FVector(0.0f, 0.0f, 30.0f);
      Cable->SetVisibility(true, true); 
   }
}

#pragma endregion

#pragma region Rewind

void ACouchFishingRod::RewindCable(float DeltaTime, float JoystickX, float JoystickY)
{
   if (Cable && LureRef)
   {
      float CircularMotion = FMath::Sqrt(FMath::Square(JoystickX) + FMath::Square(JoystickY));

      if (CircularMotion > Threshold)
      {
         float CurrentAngle = FMath::Atan2(JoystickY, JoystickX);
         
         if (PreviousAngle == -999.0f) {PreviousAngle = CurrentAngle;}
         
         float AngleDelta = FMath::Abs(CurrentAngle - PreviousAngle);

         // Normalisation
         if (AngleDelta > PI)
         {
            AngleDelta = 2 * PI - AngleDelta;
         }
         
         if (AngleDelta > 0.5f)
         {
            FVector StartPosition = SkeletalMesh->GetSocketLocation(FName("barrel"));
            FVector LurePosition = LureRef->GetActorLocation();
            
            FVector NewPositionXY = FMath::VInterpTo(FVector(LurePosition.X, LurePosition.Y, StartPosition.Z), StartPosition, DeltaTime, RewindSpeed);
            LureRef->SetActorLocation(NewPositionXY);
            
            if (FMath::Abs(LurePosition.Y - StartPosition.Y) <= 20.f)
            {
               FVector NewPositionZ = FMath::VInterpTo(LurePosition, StartPosition, DeltaTime, RewindSpeed);
               LureRef->SetActorLocation(NewPositionZ);
               LureRef->SphereComponent->SetSimulatePhysics(false);
            }
            
            if (FVector::Dist(StartPosition, LurePosition) <= StopRewindDistance)
            {
               LureRef->DestroyLure();
               LureRef = nullptr;
               Cable->DestroyComponent();
               Cable = nullptr;
            }
         }
         PreviousAngle = CurrentAngle;
      }
   }
}

void ACouchFishingRod::StopRewindCable()
{
   LureRef->SphereComponent->SetSimulatePhysics(true);
}

#pragma endregion


