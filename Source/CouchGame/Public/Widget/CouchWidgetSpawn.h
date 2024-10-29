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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	AActor* PowerChargeActor;
	UFUNCTION(BlueprintCallable)
	AActor* GetCurrentWidget();
	UFUNCTION(BlueprintCallable)
	void SpawnWidget(UClass* WidgetToSpawn, USceneComponent* AttachParent);
	UFUNCTION(BlueprintCallable)
	void DestroyWidget();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	AActor* CurrentWidget = nullptr;
};
