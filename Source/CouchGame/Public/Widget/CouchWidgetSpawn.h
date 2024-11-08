// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CouchWidgetSpawn.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UCouchWidgetSpawn : public UActorComponent
{
	GENERATED_BODY()

public:
	UCouchWidgetSpawn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> WidgetPose;
	UPROPERTY()
	TObjectPtr<AActor> PowerChargeActor;
	
	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentWidget();
	UFUNCTION(BlueprintCallable)
	void SpawnWidget(UClass* WidgetToSpawn, USceneComponent* AttachParent);
	UFUNCTION(BlueprintCallable)
	void DestroyWidget();

private:
	UPROPERTY()
	TObjectPtr<AActor> CurrentWidget = nullptr;
};
