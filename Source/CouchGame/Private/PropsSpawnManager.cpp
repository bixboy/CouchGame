
/**
 * 
 */

#include "PropsSpawnManager.h"
#include <random>

#include "CouchCannonBall.h"

APropsSpawnManager::APropsSpawnManager()
{
}

int APropsSpawnManager::GetRandomIndex()
{
	if (FishableArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("FishableArray is empty."));
		return -1; // Return an invalid index if the array is empty
	}

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> Distrib(0, FishableArray.Num() - 1);
	return Distrib(rng);
}

TScriptInterface<IFishable> APropsSpawnManager::GetPropsToSpawn(int IndexToSpawn)
{
	if (IndexToSpawn >= 0 && IndexToSpawn < FishableArray.Num())
	{
		return FishableArray[IndexToSpawn];
	}

	UE_LOG(LogTemp, Warning, TEXT("Invalid index for FishableArray."));
	return nullptr;
}

void APropsSpawnManager::SpawnProps(FTransform Transform)
{
	int RandomIndex = GetRandomIndex();
	if (RandomIndex == -1)
	{
		return; // Abort if the index is invalid
	}

	UClass* ActorClass = Cast<UClass>(GetPropsToSpawn(RandomIndex).GetObject());
	if (ActorClass && ActorClass->ImplementsInterface(UFishable::StaticClass()))
	{
		// Spawnez l'acteur
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, Transform);

		// Vérifiez si l’acteur spawné implémente bien l’interface IFishable
		if (SpawnedActor && SpawnedActor->GetClass()->ImplementsInterface(UFishable::StaticClass()))
		{
			// Cast en IFishable pour accéder aux fonctions de l'interface
			IFishable* FishableActor = Cast<IFishable>(SpawnedActor);
			if (FishableActor)
			{
				// Ajoutez ici des actions spécifiques sur FishableActor si nécessaire
			}
		}
	}
}

void APropsSpawnManager::AddFishableToArray(TScriptInterface<IFishable> FishableObject)
{
	if (FishableObject)
	{
		FishableArray.Add(FishableObject);
	}
}
