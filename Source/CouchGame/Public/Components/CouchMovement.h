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
	TObjectPtr<USplineComponent> LinePathComponent;
	UPROPERTY()
	FTimeline MoveTimeline;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float SpeedMovement;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	TObjectPtr<UCurveFloat> MoveCurve;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	bool CanMove = true;
	
	UFUNCTION()
	void MoveActor(float Alpha);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DefaultValue)
	TObjectPtr<AActor> LinePath;

	UFUNCTION()
	void StartMovement(int InputDirection);
	UFUNCTION()
	void StopMovement();

	UFUNCTION()
	bool GetCanMove() const;
	UFUNCTION()
	void SetCanMove(bool Value);
	
#pragma endregion	
};
