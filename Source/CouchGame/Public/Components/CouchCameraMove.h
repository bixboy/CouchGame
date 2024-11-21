#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CouchCameraMove.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UCouchCameraMove : public UActorComponent
{
	GENERATED_BODY()
	
protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UCouchCameraMove();
	virtual void BeginPlay() override;

public:
	void StartCameraMove();

private:
	UPROPERTY()
	FVector PointA;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	FVector DestinationPoint;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float Duration;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	float PlungeHeight;

	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	TObjectPtr<AActor> Boat1;
	UPROPERTY(EditAnywhere, Category = "Camera Movement")
	TObjectPtr<AActor> Boat2;

	float TimeElapsed;
	bool bIsMoving;

	void MoveCamera(float DeltaTime);
};
