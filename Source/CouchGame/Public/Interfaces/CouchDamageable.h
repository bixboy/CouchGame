#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CouchDamageable.generated.h"

class ACouchPlank;

UINTERFACE()
class UCouchDamageable : public UInterface
{
	GENERATED_BODY()
};

class COUCHGAME_API ICouchDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Hit(FHitResult HitResult, float RepairingTime = 0.f, float Scale = 0.f);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int GetBoatTeam();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetPercentRepair();
};
