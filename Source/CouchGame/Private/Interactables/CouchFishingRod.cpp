#include "Interactables/CouchFishingRod.h"
#include "Characters/CouchCharacter.h"
#include "CouchLure.h"
#include "Components/CouchChargePower.h"
#include "Interfaces/CouchInteractable.h"
#include "Kismet/GameplayStatics.h"
#include "Interactables/CouchPickableMaster.h"

#pragma region Setup

ACouchFishingRod::ACouchFishingRod()
{
   PrimaryActorTick.bCanEverTick = true;
   ChargePower = CreateDefaultSubobject<UCouchChargePower>(TEXT("ChargePower"));
   WidgetSpawner = CreateDefaultSubobject<UCouchWidgetSpawn>(TEXT("WidgerSpawner"));

   SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
   RootComponent = SkeletalMesh;
   WidgetPose = CreateDefaultSubobject<USceneComponent>(TEXT("WidgetPose"));
   WidgetPose->SetupAttachment(SkeletalMesh);
}

void ACouchFishingRod::SetupFishingRod(TObjectPtr<ACouchCharacter> Player, int Team)
{
   CurrentTeam = Team;
   CurrentPlayer = Player;
   if (CurrentPlayer)
   {
      AttachToComponent(CurrentPlayer->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripAttach"));
      CurrentPlayer->AnimationManager->IsFishing = true;
      //FVector SocketLocation = FVector(SkeletalMesh->GetSocketTransform(FName("GripPoint"), RTS_Component).GetLocation());
      //AddActorLocalOffset(SocketLocation);
   }
}

void ACouchFishingRod::Tick(float DeltaSeconds)
{
   Super::Tick(DeltaSeconds);
   if (isPlayerFishing)
   {
      RewindCable(DeltaSeconds);
      if (!CurrentPlayer->AnimationManager->IsFishingPull)
      {
         CurrentPlayer->AnimationManager->IsFishingPull = true;
      }
   }
   else
   {
      if (CurrentPlayer->AnimationManager->IsFishingPull)
      {
         CurrentPlayer->AnimationManager->IsFishingPull = false;
      }
   }
}

#pragma endregion

#pragma region ChargingPower

// Start Charge
void ACouchFishingRod::StartChargeActor_Implementation()
{
   ICouchInteractable::StartChargeActor_Implementation();
   if(!IsInCharge)
   {
      WidgetSpawner->SpawnWidget(PowerChargeWidget, WidgetPose, false);
      ChargePower->StartCharging(SkeletalMesh, WidgetSpawner, false, CurrentPlayer);
      
      CurrentPlayer->AnimationManager->IsFishingStart = true;
      IsInCharge = true;
   }
}

// Stop Charge
void ACouchFishingRod::StopChargeActor_Implementation()
{
   ICouchInteractable::StopChargeActor_Implementation();
   if (IsInCharge)
   {
      ChargePower->StopCharging();
      
      CurrentPlayer->AnimationManager->IsFishingStart = false;
      CurrentPlayer->AnimationManager->IsFishingRelease = true;
      IsInCharge = false;
   }
}

#pragma endregion 

#pragma region Spawn & Init Lure

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
      LureRef->DetachAttachedObject();
      DestroyLureAndCable();
   }

   // Spawn Lure
   FTransform SpawnTransform = FTransform(SuggestedVelocity.Rotation(), SkeletalMesh->GetSocketLocation(FName("barrel")));
   if (LureRef = GetWorld()->SpawnActor<ACouchLure>(Lure, SpawnTransform))
   {
      LureRef->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
      InitializeCableAndConstraint();
      LureRef->Initialize(SuggestedVelocity, this);
      CurrentPlayer->AnimationManager->IsFishingRelease = false;
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
      //Cable->NumSegments = 17.f;
      
      float Distance = FVector::Dist(SkeletalMesh->GetSocketLocation(FName("barrel")), ChargePower->TargetLocation);
      Cable->CableLength = Distance;

      // Attache le cable
      if (LureRef && LureRef->SphereComponent && SkeletalMesh)
      {
         Cable->SetAttachEndToComponent(LureRef->SphereComponent);
         Cable->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("barrel"));
         Cable->SetAttachEndTo(LureRef, FName(TEXT("TopMesh")), FName(TEXT("barrel")));
      }

      Cable->EndLocation = FVector(0.0f, 0.0f, 30.0f);
      Cable->SetVisibility(true, true); 
   }
}

#pragma endregion

#pragma region Rewind

void ACouchFishingRod::RewindCable(float DeltaTime)
{
   if (LureRef && !InQte)
   {
      FVector StartPosition = SkeletalMesh->GetSocketLocation(FName("barrel"));
      FVector LurePosition = LureRef->GetActorLocation();
            
      FVector TargetPositionXY = FVector(StartPosition.X, StartPosition.Y, LurePosition.Z);
      FVector NewPositionXY = FMath::VInterpConstantTo(LurePosition, TargetPositionXY, DeltaTime, RewindSpeed);
      LureRef->SetActorLocation(NewPositionXY);
      
      // Si la position est proche de start position, monter sur l'axe Z
      if (FMath::Abs(LurePosition.Y - StartPosition.Y) <= StopRewindZ)
      {
         FVector TargetPositionZ = FVector(StartPosition.X, StartPosition.Y, StartPosition.Z);
         FVector NewPositionZ = FMath::VInterpConstantTo(NewPositionXY, TargetPositionZ, DeltaTime, RewindSpeed);
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
}

void ACouchFishingRod::StopRewindCable()
{
   if (LureRef && LureRef->SphereComponent && !InQte)
   {
      LureRef->SphereComponent->SetSimulatePhysics(true);
   }
   else if (InQte)
   {
      RewindQte();
   }
}

#pragma endregion

#pragma region Spawn Pickable Object

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
   TObjectPtr<ACouchPickableMaster> PickableActor = GetWorld()->SpawnActor<ACouchPickableMaster>(LureRef->GetFishingObject(), Transform);

   TArray<TObjectPtr<AActor>> ActorToIgnore;
   ActorToIgnore.Add(this);
   ActorToIgnore.Add(LureRef);
   ActorToIgnore.Add(CurrentPlayer);
   PickableActor->CouchProjectile->Initialize(SuggestedVelocity, ActorToIgnore);

   DestroyLureAndCable();
   CurrentPlayer->DestroyFishingRod();
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

#pragma region QTE Rewind

void ACouchFishingRod::StartQte()
{
   if (!InQte)
   {
      InQte = true;
   }
}

void ACouchFishingRod::StopQte()
{
   if (InQte)
   {
      InQte = false;
   }
}

void ACouchFishingRod::RewindQte()
{
   if (LureRef->GetFishingObjectActor()->GetQtePercent() <= 1 || LureRef->GetFishingObjectActor()->GetQtePercent() >= 0 )
   {
      if(CurrentTeam == 1 && LureRef)
      {
         LureRef->GetFishingObjectActor()->UpdatePercent(-QtePercent);
      }
      else if(CurrentTeam == 2 && LureRef)
      {
         LureRef->GetFishingObjectActor()->UpdatePercent(QtePercent);
      }  
   }
}

#pragma endregion

#pragma region Getter
// Is Used
bool ACouchFishingRod::IsUsedByPlayer_Implementation() {return ICouchInteractable::IsUsedByPlayer_Implementation();}

// Get Player
TObjectPtr<ACouchCharacter> ACouchFishingRod::GetCharacter() const {return CurrentPlayer;}

// Get Team
int ACouchFishingRod::GetTeam() const {return CurrentTeam;}

#pragma endregion

void ACouchFishingRod::DestroyLureAndCable()
{
   if (LureRef)
   {
      LureRef->DestroyLure();
      LureRef = nullptr;
   }
   if (Cable)
   {
      Cable->DestroyComponent();
      Cable = nullptr;
   }
}

void ACouchFishingRod::DestroyFishingRod()
{
   if (LureRef)
   {
      LureRef->DetachAttachedObject();
      LureRef->Destroy();
      LureRef = nullptr;
   }
   if (Cable)
   {
      Cable->DestroyComponent();
      Cable = nullptr;
   }
   if (WidgetSpawner->GetCurrentWidget())
   {
      WidgetSpawner->DestroyWidget();
   }
}
