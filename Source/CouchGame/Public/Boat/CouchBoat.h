#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CouchBoat.generated.h"

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
	float GetBoatLife() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BoatMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat", meta = (AllowPrivateAccess = "true"))
	float BoatStartLife = 100.f;

	UPROPERTY()
	float BoatLife;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Planks;

	
	

};
