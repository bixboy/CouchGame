// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEffect/Effects/Mud/Mud.h"
#include "Characters/CouchCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interactables/CouchPickableMaster.h"


// Sets default values
AMud::AMud()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMeshComponent;
    BoxComponent->SetupAttachment(StaticMeshComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMud::OnActorBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMud::OnActorEndOverlap);
}

void AMud::Init(float InPlayerSlowingSpeed, FVector2f InItemsSlowingSpeed)
{
	PlayerSlowingSpeed = InPlayerSlowingSpeed;
	ItemsSlowingSpeed = InItemsSlowingSpeed;
}

void AMud::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ACouchCharacter* Character = Cast<ACouchCharacter>(OtherActor); Character)
    {
        CharactersBaseSpeed.Add(Character, Character->GetCharacterMovement()->MaxWalkSpeed);
        Character->GetCharacterMovement()->MaxWalkSpeed = PlayerSlowingSpeed;
    }
    else if (ACouchPickableMaster* Item = Cast<ACouchPickableMaster>(OtherActor))
    {
        if (!Item) return;

        UPrimitiveComponent* PrimitiveComponent = Item->FindComponentByClass<UPrimitiveComponent>();
        if (!PrimitiveComponent) return;

        FItemPhysicsVelocity NewItem;
        NewItem.Pickable = Item;
        NewItem.LinearDamping = PrimitiveComponent->GetLinearDamping();
        NewItem.AngularDamping = PrimitiveComponent->GetAngularDamping();
        PickableBaseVelocities.Add(NewItem);

        PrimitiveComponent->SetLinearDamping(ItemsSlowingSpeed.X);
        PrimitiveComponent->SetAngularDamping(ItemsSlowingSpeed.Y);
    }
}

void AMud::OnActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ACouchCharacter* Character = Cast<ACouchCharacter>(OtherActor); Character)
    {
        if (CharactersBaseSpeed.Contains(Character))
        {
            Character->GetCharacterMovement()->MaxWalkSpeed = CharactersBaseSpeed[Character];
            CharactersBaseSpeed.Remove(Character);
        }
    }
    else if (ACouchPickableMaster* Item = Cast<ACouchPickableMaster>(OtherActor))
    {
        FItemPhysicsVelocity TempItem;
        TempItem.Pickable = Item;
        FItemPhysicsVelocity* FoundItem = PickableBaseVelocities.FindByPredicate([&](const FItemPhysicsVelocity& Velocity)
        {
            return Velocity == TempItem;
        });

        if (FoundItem)
        {
            UPrimitiveComponent* PrimitiveComponent = Item->FindComponentByClass<UPrimitiveComponent>();
            if (!PrimitiveComponent) return;

            PrimitiveComponent->SetLinearDamping(FoundItem->LinearDamping);
            PrimitiveComponent->SetAngularDamping(FoundItem->AngularDamping);

            PickableBaseVelocities.RemoveSingle(*FoundItem);
        }
    }
}

bool FItemPhysicsVelocity::operator==(const FItemPhysicsVelocity& Other) const
{
	return Pickable == Other.Pickable;
}


