#include "Components/CouchProjectile.h"

#include "ShadowProjectile.h"
#include "Interactables/CouchPickableMaster.h"
#include "Interfaces/CouchPickable.h"

UCouchProjectile::UCouchProjectile()
{
	PrimaryComponentTick.bCanEverTick = true;
}
// Init
void UCouchProjectile::Initialize(const FVector& LaunchVelocity, const TArray<AActor*> ActorsToIgnore, bool UnableCollision)
{
	Velocity = LaunchVelocity;
	Location = GetOwner()->GetActorLocation();
	TimeElapsed = 0.0f;
	CanMove = true;
	UPrimitiveComponent* PrimitiveComponent = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (PrimitiveComponent)
	{
		PrimitiveComponent->SetSimulatePhysics(false);
	}
	CollisionIsActive = UnableCollision;
	IgnoredActors = ActorsToIgnore;
}

// tick movement & collision
void UCouchProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(ReactivatePhysicsCountdown > 0.0f)
	{
		ReactivatePhysicsCountdown -= DeltaTime;
		if(ReactivatePhysicsCountdown <= 0.0f)
		{
			UPrimitiveComponent* PrimitiveComponent = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
			if (PrimitiveComponent && PrimitiveComponent->IsSimulatingPhysics())
			{
				PrimitiveComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
			}			
		}
	}

	if (CanMove)
	{
		if (GetOwner()->IsA(ACouchPickableMaster::StaticClass()))
		{
			if (GetOwner()->Implements<UCouchPickable>() && !ICouchPickable::Execute_IsPickable(GetOwner()))
			{
				SetCanMove(false);
			}
		}
		// Movement
		TimeElapsed += DeltaTime;
		FVector NewLocation = Location + (Velocity * SpeedMultiplier * TimeElapsed) + (FVector(0, 0, Gravity) * TimeElapsed * TimeElapsed * 0.5f);

		FHitResult HitResult;
		FVector Start = Location;
		FVector End = NewLocation;

		// Collisions
		if (CollisionIsActive)
		{
			FCollisionQueryParams CollisionParams;
		
			for (AActor* ActorToIgnore : IgnoredActors)
				if (ActorToIgnore)
				{
					CollisionParams.AddIgnoredActor(ActorToIgnore);
					CollisionParams.AddIgnoredActor(GetOwner());	
				}

			bool bHit = GetWorld()->SweepSingleByChannel(
				HitResult, 
				Start, 
				End, 
				FQuat::Identity, 
				ECC_Visibility, 
				FCollisionShape::MakeSphere(15.f),
				CollisionParams
			);
		
			if (bHit)
			{
				NewLocation = HitResult.Location;
				Velocity = FVector::ZeroVector;
				SetCanMove(false);
				UPrimitiveComponent* PrimitiveComponent = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
				if (PrimitiveComponent)
				{
					PrimitiveComponent->SetSimulatePhysics(true);
					PrimitiveComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
					ReactivatePhysicsCountdown = ReactivatePhysicsDelay;
				}
				FString ActorName = HitResult.GetActor()->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Actor hit: %s"), *ActorName));
			}

			if (!bHit)
			{
				GetOwner()->SetActorLocation(NewLocation);
			}
		}
		else
		{
			GetOwner()->SetActorLocation(NewLocation);
		}
	}
}

bool UCouchProjectile::GetCanMove() const {return CanMove;}

void UCouchProjectile::SetCanMove(bool Value) {CanMove = Value;}

