#include "Components/CouchProjectile.h"

#include "ShadowProjectile.h"

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

	CollisionIsActive = UnableCollision;
	IgnoredActors = ActorsToIgnore;
}

// tick movement & collision
void UCouchProjectile::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanMove)
	{
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

