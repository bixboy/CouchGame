// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CouchCharacterState.h"
#include "CouchCharacterStateDash.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UCouchCharacterStateDash : public UCouchCharacterState
{
	GENERATED_BODY()

public:
	virtual ECouchCharacterStateID GetStateID() override;
		
	virtual void StateEnter(ECouchCharacterStateID PreviousStateID) override;

	virtual void StateExit(ECouchCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	float DashDuration =.3f;
	UPROPERTY(EditAnywhere)
	float DashForce = 100;
	UPROPERTY()
	float DashTimer;
};
