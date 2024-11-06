// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CouchCharacter.h"
#include "GameFramework/Actor.h"
#include "CouchTeamCard.generated.h"

UCLASS()
class COUCHGAME_API ACouchTeamCard : public AActor
{
	GENERATED_BODY()

public:
	ACouchTeamCard();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<ACouchCharacter*> Characters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Mesh;
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FTimerHandle SpawnTimerHandle;

	UPROPERTY(EditAnywhere)
	UClass* CharacterToSpawn;
	
	void SpawnCharacter();
	
	UPROPERTY()
	int PlayerNumber;
	UPROPERTY()
	int CurrentPlayer = 0;
};
