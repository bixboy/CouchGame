// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CouchWidgetSpawn.generated.h"

class ACouchWidget3D;

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
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ACouchWidget3D* GetCurrentWidget();
	
	UFUNCTION(BlueprintCallable)
	void SpawnWidget(TSubclassOf<ACouchWidget3D> WidgetToSpawn, USceneComponent* AttachParent);
	
	UFUNCTION(BlueprintCallable)
	void DestroyWidget();

private:
	UPROPERTY()
	TObjectPtr<ACouchWidget3D> CurrentWidget = nullptr;
};
