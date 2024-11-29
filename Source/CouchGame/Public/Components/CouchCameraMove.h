#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "CouchCameraMove.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UCouchCameraMove : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UCouchCameraMove();
	virtual void BeginPlay() override;

public:
	void StartCameraMove();

private:
	UPROPERTY()
	FVector PointA;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	FVector DestinationPoint;

	UPROPERTY()
	FTimeline MoveTimeline;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	TObjectPtr<UCurveFloat> MoveCurve;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float Speed;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float PlungeHeight;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	TObjectPtr<AActor> Boat1;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	TObjectPtr<AActor> Boat2;

	bool bIsMoving;

	UFUNCTION()
	void MoveCamera(float DeltaTime);
	UFUNCTION()
	void ReversMoveCamera();
};
