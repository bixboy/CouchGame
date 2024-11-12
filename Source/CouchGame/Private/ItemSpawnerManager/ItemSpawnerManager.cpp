// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnerManager/ItemSpawnerManager.h"

#include "IDetailTreeNode.h"
#include "Components/BoxComponent.h"
#include "Interactables/CouchPickableMaster.h"
#include "Kismet/KismetMathLibrary.h"




// Sets default values
AItemSpawnerManager::AItemSpawnerManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SpawnBox = CreateDefaultSubobject<UBoxComponent>("SpawnBox");
	DestroyBox = CreateDefaultSubobject<UBoxComponent>("DestroyBox");

	SpawnBox->SetupAttachment(RootComponent);
	DestroyBox->SetupAttachment(RootComponent);

	DestroyBox->OnComponentBeginOverlap.AddDynamic(this, &AItemSpawnerManager::OnItemBeginOverlapDestoryBox);
	
}

// Called when the game starts or when spawned
void AItemSpawnerManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemSpawnerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentSpawnedItems.Num() < MaxItemSpawnedAtSameTime && !ItemsToSpawn.IsEmpty())
	{
		Timer += DeltaTime;
		if(Timer >= SpawnRate)
		{
			SpawnItem();
			Timer = 0;
		}
	}
}

void AItemSpawnerManager::OnItemBeginOverlapDestoryBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Item Spawner Destroyed");
	if (OtherActor->IsA(ACouchPickableMaster::StaticClass()))
	{
		ACouchPickableMaster* Item = Cast<ACouchPickableMaster>(OtherActor);
		DestroyItem(Item);
	}
	else if (!OtherActor->IsA(ACouchCharacter::StaticClass()))
	{
		OtherActor->Destroy();
	}
}

void AItemSpawnerManager::DestroyItem(ACouchPickableMaster* ItemToDestroy, bool isDestroyByItemSpawnerManager)
{
	CurrentSpawnedItems.Remove(ItemToDestroy);
	if (isDestroyByItemSpawnerManager)
	{
		ItemToDestroy->Destroy();
	}
}

void AItemSpawnerManager::SpawnItem()
{
	if (CurrentSpawnedItems.Num() >= MaxItemSpawnedAtSameTime || ItemsToSpawn.IsEmpty()) return;
	TSubclassOf<ACouchPickableMaster> ItemToSpawn = RandomSpawn();
	// switch (SpawnCycle)
	// {
	// 	case EItemSpawnerCycle::Random:
	// 	{
	// 		ItemToSpawn = RandomSpawn();
	// 		break;
	// 	}
	// 	case EItemSpawnerCycle::Loop:
	// 	{
	// 		ItemToSpawn = LoopSpawn();
	// 		break;
	// 	}
	// 	case EItemSpawnerCycle::PingPong:
	// 	{
	// 		ItemToSpawn = PingPongSpawn();
	// 		break;
	// 	}
	// 	default:
	// 		break;
	// }
	if (!ItemToSpawn || !SpawnBox) return;
	FVector BoxExtent = SpawnBox->GetScaledBoxExtent();
	FVector RandomLocation = UKismetMathLibrary::RandomPointInBoundingBox(SpawnBox->GetComponentLocation(), BoxExtent);

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(RandomLocation);
	SpawnTransform.SetRotation(FQuat::Identity);
	
	TObjectPtr<ACouchPickableMaster> Item = GetWorld()->SpawnActor<ACouchPickableMaster>(ItemToSpawn, SpawnTransform);
	CurrentSpawnedItems.Add(Item);
}

TSubclassOf<ACouchPickableMaster> AItemSpawnerManager::RandomSpawn()
{
	float randomChance = FMath::FRandRange(0.f, 1.f);
	TArray<TSubclassOf<ACouchPickableMaster> > ItemsWhichCanBeSpawn;

	for (const auto Item : ItemsToSpawn)
	{
		if (Item.Value > randomChance){
		{
			ItemsWhichCanBeSpawn.Add(Item.Key);
		}}
	}
	if (ItemsWhichCanBeSpawn.IsEmpty()) return nullptr;
	return ItemsWhichCanBeSpawn[FMath::RandRange(0,ItemsWhichCanBeSpawn.Num()-1)];
}

// TSubclassOf<ACouchPickableMaster> AItemSpawnerManager::LoopSpawn()
// {
// 	TSubclassOf<ACouchPickableMaster> Item = ItemsToSpawn[Iterator];
// 	Iterator++;
// 	if (Iterator >= ItemsToSpawn.Num()) Iterator = 0;
// 	return Item;
// }
//
// TSubclassOf<ACouchPickableMaster> AItemSpawnerManager::PingPongSpawn()
// {
// 	TSubclassOf<ACouchPickableMaster> Item = ItemsToSpawn[Iterator];
// 	Iterator += PingPongDirection;
// 	if (Iterator >= ItemsToSpawn.Num() || Iterator < 0)
// 	{
// 		PingPongDirection *= -1;
// 		Iterator += PingPongDirection;
// 	}
// 	return Item;
// }

