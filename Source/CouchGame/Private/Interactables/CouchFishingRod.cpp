#include "Interactables/CouchFishingRod.h"
#include "Characters/CouchCharacter.h"
#include "CouchLure.h"
#include "Components/CouchChargePower.h"
#include "Interfaces/CouchInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "Interactables/CouchPickableMaster.h"

ACouchFishingRod::ACouchFishingRod()
{
   PrimaryActorTick.bCanEverTick = false;
   ChargePower = CreateDefaultSubobject<UCouchChargePower>(TEXT("ChargePower"));

   SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
   RootComponent = SkeletalMesh;
}


void ACouchFishingRod::SetupFishingRod(ACouchCharacter* Player)
{
   CurrentPlayer = Player;
   if (CurrentPlayer)
   {
      AttachToComponent(CurrentPlayer->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripAttach"));
      //FVector SocketLocation = FVector(SkeletalMesh->GetSocketTransform(FName("GripPoint"), RTS_Component).GetLocation());
      //AddActorLocalOffset(SocketLocation);
   }
}

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

#pragma region SpawnLure & InitLure

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

   if (LureRef)
   {
      DestroyLureAndCable();
   }
      
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
   if (LureRef)
   {
       float CircularMotion = FMath::Sqrt(FMath::Square(JoystickX) + FMath::Square(JoystickY));

       if (CircularMotion > Threshold)
       {
           // Compare l'angle actuel a previous Angle
           float CurrentAngle = FMath::Atan2(JoystickY, JoystickX);
           if (PreviousAngle == -999.0f) { PreviousAngle = CurrentAngle; }
           
           float AngleDelta = FMath::Abs(CurrentAngle - PreviousAngle);
           AngleDelta = (AngleDelta > PI) ? 2 * PI - AngleDelta : AngleDelta;
          
           if (AngleDelta > 0.5f)
           {
               FVector StartPosition = SkeletalMesh->GetSocketLocation(FName("barrel"));
               FVector LurePosition = LureRef->GetActorLocation();
               
               FVector TargetPositionXY = FVector(StartPosition.X, StartPosition.Y, LurePosition.Z);
               FVector NewPositionXY = FMath::VInterpTo(LurePosition, TargetPositionXY, DeltaTime, RewindSpeed);
               LureRef->SetActorLocation(NewPositionXY);

               // Si la position est proche de start position, monter
               if (FMath::Abs(LurePosition.Y - StartPosition.Y) <= 5.f)
               {
                   FVector TargetPositionZ = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z);
                   FVector NewPositionZ = FMath::VInterpTo(NewPositionXY, TargetPositionZ, DeltaTime, RewindSpeed);
                   LureRef->SetActorLocation(NewPositionZ);
                   
                   LureRef->SphereComponent->SetSimulatePhysics(false); 
               }
              
              if (FVector::Dist(StartPosition, LurePosition) <= StopRewindDistance)
               {
                  if (LureRef->GetFishingObject())
                  {
                     SpawnPickableObject();  
                  }
                   DestroyLureAndCable();
               }
           }
          PreviousAngle = CurrentAngle;
       }
   }
}

void ACouchFishingRod::StopRewindCable() {LureRef->SphereComponent->SetSimulatePhysics(true);}

#pragma endregion

#pragma region SpawnPickableObject

void ACouchFishingRod::SpawnPickableObject()
{
   FVector StartLocation = SkeletalMesh->GetSocketLocation(FName("barrel"));
   FVector TargetLocation = GetRandomPos(200.f, 500.f, 100.f);
   FVector SuggestedVelocity;
   
   UGameplayStatics::SuggestProjectileVelocity_CustomArc(
    this,
    SuggestedVelocity,
    StartLocation,
    TargetLocation,
    0.f,
    0.5f
 ); 
   
   FTransform Transform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
   TObjectPtr<ACouchPickableMaster> PickableActor = GetWorld()->SpawnActor<ACouchPickableMaster>(PickableObject, Transform);

   TArray<TObjectPtr<AActor>> ActorToIgnore;
   ActorToIgnore.Add(this);
   ActorToIgnore.Add(LureRef);
   ActorToIgnore.Add(CurrentPlayer);
   PickableActor->CouchProjectile->Initialize(SuggestedVelocity, ActorToIgnore);
}

FVector ACouchFishingRod::GetRandomPos(const float MinDistance, const float MaxDistance, const float Width)
{
   if (!CurrentPlayer)
   {
      return FVector::ZeroVector;
   }
   
   FRotator BackwardRotation = CurrentPlayer->GetMesh()->GetComponentRotation();
   BackwardRotation.Yaw += 90.0f;
   
   FVector BackwardDirection = -FRotationMatrix(BackwardRotation).GetUnitAxis(EAxis::X);
   FVector PlayerLocation = SkeletalMesh->GetSocketLocation(FName("barrel"));

   float DistanceBehind = FMath::FRandRange(MinDistance, MaxDistance);
   FVector CenterPosition = PlayerLocation + (BackwardDirection * DistanceBehind);

   float OffsetX = FMath::FRandRange(-Width / 2.0f, Width / 2.0f);
   float OffsetY = FMath::FRandRange(-Width / 2.0f, Width / 2.0f);
   FVector RandomPosition = CenterPosition + FVector(OffsetX, OffsetY, 0.0f);

   return RandomPosition;
}

#pragma endregion

bool ACouchFishingRod::IsUsedByPlayer_Implementation() {return ICouchInteractable::IsUsedByPlayer_Implementation();}

void ACouchFishingRod::DestroyLureAndCable()
{
   if (LureRef)
   {
      LureRef->Destroy();
      LureRef = nullptr;
   }
   if (Cable)
   {
      Cable->DestroyComponent();
      Cable = nullptr;
   }
}


