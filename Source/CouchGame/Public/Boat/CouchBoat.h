#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchBoat.generated.h"

UENUM(Blueprintable)
enum class EBoatTeam : uint8
{
	Team1,
	Team2
};

class ABoatFloor;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class COUCHGAME_API ACouchBoat : public AActor
{
	GENERATED_BODY()

public:
	ACouchBoat();
	
	virtual void BeginPlay() override;
	void BoatDamage(float DamageAmount);
	void BoatRepair(float HealAmount);
	void BoatRepair();
	void SinkBoatAndGameOver();
	UFUNCTION(BlueprintCallable)
	float GetBoatLife() const;

private:
	UPROPERTY(EditAnywhere)
	ABoatFloor* BoatFloor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BoatMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float BoatStartLife = 100.f;

	UPROPERTY(VisibleAnywhere)
	float BoatLife;

	UPROPERTY(EditAnywhere)
	FVector2D BoatMinAndMaxDamagePerSecond;
	UPROPERTY(EditAnywhere)
	float HealAmountPerHitRepaired;

	UPROPERTY(EditAnywhere)
	EBoatTeam Team;
};

