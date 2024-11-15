#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Widget/CouchWidgetSpawn.h"
#include "CouchChargePower.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UCouchChargePower : public UActorComponent
{
	GENERATED_BODY()


public:
	UCouchChargePower();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;


#pragma region Power Charge


public:
	UFUNCTION(BlueprintCallable)
	void StartCharging(USkeletalMeshComponent* MeshComp, UCouchWidgetSpawn* WidgetSpawner = nullptr);
	UFUNCTION(BlueprintCallable)
	void StopCharging();

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, Category = DefaultValue)
	UClass* PowerChargeWidget;
  
private:   
	UPROPERTY()
	FTimeline PowerTimeline;
  
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> PowerCurve;
  
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float MaxPower = 500.f;
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float MinPower = 100.f;
	UPROPERTY()
	float CurrentPower;
	
	UPROPERTY(EditAnywhere, Category = DefaultValue)
	float SpeedCharge = 1.f;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> Mesh;
	UPROPERTY()
	TObjectPtr<UCouchWidgetSpawn> ChargeWidget;

	UFUNCTION()
	float UpdatePower(float Alpha);
  
	UFUNCTION()
	FVector LineTrace();

#pragma endregion
};
