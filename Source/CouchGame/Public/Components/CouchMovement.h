// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "CouchMovement.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UCouchMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	UCouchMovement();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

#pragma region Movement
private:
	UPROPERTY()
	FTimeline MoveTimeline;
	UPROPERTY(EditAnywhere)
	UCurveFloat* MoveCurve;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float SpeedMovement;
	
	UPROPERTY()
	USplineComponent* LinePathComponent;
	
	UFUNCTION()
	void MoveActor(float Alpha);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	AActor* LinePath;

	UFUNCTION()
	void StartMovement(int InputDirection);
	UFUNCTION()
	void StopMovement();
	
#pragma endregion	
};
