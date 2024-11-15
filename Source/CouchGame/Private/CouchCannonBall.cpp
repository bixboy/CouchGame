// Fill out your copyright notice in the Description page of Project Settings.

#include "CouchCannonBall.h"

#include "CouchStaticCanonBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/CouchDamageable.h"
#include "ProjectileEffect/CouchProjectileEffect.h"


ACouchCannonBall::ACouchCannonBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	
	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base"));
	Base->SetupAttachment(RootComponent);
	Top = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top"));
	Top->SetupAttachment(Base);
	Down = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down"));
	Down->SetupAttachment(Base);

	Sphere->OnComponentHit.AddDynamic(this, &ACouchCannonBall::OnCannonBallHit);
}


void ACouchCannonBall::BeginPlay()
{
	Super::BeginPlay();

}

bool ACouchCannonBall::HasEffectWithExecuteTime(ECouchProjectileExecuteTime ExecuteTime)
{
	for (auto ProjectileEffect : ProjectileEffects)
	{
		if (ProjectileEffect && ProjectileEffect->ExecuteTime == ExecuteTime)
		{
			return true;
		}
	}
	return false;
}

void ACouchCannonBall::ExecuteEffectWithExecuteTime(ECouchProjectileExecuteTime ExecuteTime)
{
	for (auto ProjectileEffect : ProjectileEffects)
	{
		if (ProjectileEffect && ProjectileEffect->ExecuteTime == ExecuteTime)
		{
			ProjectileEffect->ExecuteEffect();
			ProjectileEffects.Remove(ProjectileEffect);
		}
	}
}

void ACouchCannonBall::InitEffect(ACouchCannonBall* CanonBall, ACouchPickableCannonBall* PickCannonBall,
	AActor* ActorToInteractWith, FHitResult HitResult)
{
	for (auto Effect : ProjectileEffects)
	{
		if (Effect)
		{
			if (CanonBall) Effect->Init(CanonBall);
			if (PickCannonBall) Effect->Init(PickCannonBall);
			if (ActorToInteractWith) Effect->Init(ActorToInteractWith);
			if (HitResult.IsValidBlockingHit()) Effect->Init(HitResult);
		}
	}
}

void ACouchCannonBall::InitEffectWithExecuteTime(ECouchProjectileExecuteTime ExecuteTime, ACouchCannonBall* CanonBall,
	ACouchPickableCannonBall* PickCannonBall, AActor* ActorToInteractWith, FHitResult HitResult)
{
	for (auto Effect : ProjectileEffects)
	{
		if (Effect && Effect->ExecuteTime == ExecuteTime)
		{
			if (CanonBall) Effect->Init(CanonBall);
			if (PickCannonBall) Effect->Init(PickCannonBall);
			if (ActorToInteractWith) Effect->Init(ActorToInteractWith);
			if (HitResult.IsValidBlockingHit()) Effect->Init(HitResult);
		}
	}
}

void ACouchCannonBall::Initialize(const FVector& LaunchVelocity)
{
	Velocity = LaunchVelocity;  // Définir la vélocité initiale
	Location = GetActorLocation(); // Obtenir la position initiale
	TimeElapsed = 0.0f; // Réinitialiser le temps écoulé
}

void ACouchCannonBall::InitCanonBall(TObjectPtr<ACouchStaticCanonBall> StaticCannonBall)
{
	SetActorTransform(StaticCannonBall->GetActorTransform());
	Base->SetStaticMesh(StaticCannonBall->Base->GetStaticMesh());
	
	Top->SetStaticMesh( StaticCannonBall->Top->GetStaticMesh());
	Top->SetRelativeTransform(StaticCannonBall->Top->GetRelativeTransform());
	
	Down->SetStaticMesh( StaticCannonBall->Down->GetStaticMesh());
	Down->SetRelativeTransform(StaticCannonBall->Down->GetRelativeTransform());

	ProjectileEffectsClass = StaticCannonBall->ProjectileEffects;
	this->PickableCannonBall = StaticCannonBall->PickableCannonBall;
	
	if (!ProjectileEffectsClass.IsEmpty())
	{
		for (auto ProjectileEffectClass : ProjectileEffectsClass)
		{
			auto ProjectileEffect = GetWorld()->SpawnActor<ACouchProjectileEffect>(ProjectileEffectClass);
			ProjectileEffects.Add(ProjectileEffect);
		}
		InitEffect(this);
	}
	if (!ProjectileEffects.IsEmpty())
	{
		if (HasEffectWithExecuteTime(ECouchProjectileExecuteTime::OnLaunch))
		{
			ExecuteEffectWithExecuteTime(ECouchProjectileExecuteTime::OnLaunch);
		}
	}
}

void ACouchCannonBall::OnCannonBallHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);
	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	Sphere->SetSimulatePhysics(true);

	// Pour éviter que le bullet de cannon ne soit bloqué dans les collisions si jamais il touche le rebord avant
	// Calculer la direction opposée à la collision
	FVector RepulsionDirection = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();

	// Calculer une nouvelle position en se déplaçant légèrement hors de la collision
	float RepulsionDistance = 10.0f;
	FVector NewLocation = GetActorLocation() + RepulsionDirection * RepulsionDistance;
        
	// Déplacer l'Actor vers la nouvelle position
	SetActorLocation(NewLocation);
	

	if (OtherActor->Implements<UCouchDamageable>())
	{
		if (!ProjectileEffects.IsEmpty())
		{
			bool HasEffectWithExecuteTimeDelay;
			bool HasEffectWithExecuteTimeImpactWithHit;
			bool HasEffectWithExecuteTimeImpactWithoutHit;
			if(HasEffectWithExecuteTimeDelay = HasEffectWithExecuteTime(ECouchProjectileExecuteTime::OnDelay); HasEffectWithExecuteTimeDelay)
			{
				ACouchPickableCannonBall* PickCannonBall = GetWorld()->SpawnActor<ACouchPickableCannonBall>
				(PickableCannonBall->GetClass(), GetActorTransform());
				InitEffectWithExecuteTime(ECouchProjectileExecuteTime::OnDelay, this, PickCannonBall, OtherActor, Hit);
				if (!HasEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithHit) &&
				!HasEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithoutHit)) Destroy();
			}
			if (HasEffectWithExecuteTimeImpactWithHit = HasEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithHit); HasEffectWithExecuteTimeImpactWithHit)
			{
				InitEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithHit, this, nullptr, OtherActor, Hit);
				ExecuteEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithHit);
				ICouchDamageable::Execute_Hit(OtherActor, Hit,0,0);
			}
			if (HasEffectWithExecuteTimeImpactWithoutHit = HasEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithoutHit); HasEffectWithExecuteTimeImpactWithoutHit)
			{
				InitEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithoutHit, this, nullptr, OtherActor, Hit);
				ExecuteEffectWithExecuteTime(ECouchProjectileExecuteTime::OnImpactWithoutHit);
			}
			if (!HasEffectWithExecuteTimeDelay
				&& !HasEffectWithExecuteTimeImpactWithHit
				&& !HasEffectWithExecuteTimeImpactWithoutHit)
			{
				ICouchDamageable::Execute_Hit(OtherActor,Hit,0,0);
				Destroy();
			}
			if (ProjectileEffects.IsEmpty())
			{
				Destroy();
			}
		}	
		else if (!HasEffectWithExecuteTime(ECouchProjectileExecuteTime::OnDelay))
		{
			ICouchDamageable::Execute_Hit(OtherActor, Hit,0,0);
			Destroy();
		}
	}
}

void ACouchCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Sphere->IsSimulatingPhysics())
	{
		TimeElapsed += DeltaTime;
		FVector NewLocation = Location + (Velocity * TimeElapsed) + (FVector(0, 0, Gravity) * TimeElapsed * TimeElapsed * 0.5f);
		SetActorLocation(NewLocation);
		HasBeenLaunched = true;
	}
}

