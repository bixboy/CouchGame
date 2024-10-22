// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CouchCharacterState.h"
#include "CouchCharacterStateWalk.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME_API UCouchCharacterStateWalk : public UCouchCharacterState
{
	GENERATED_BODY()

public:
	virtual ECouchCharacterStateID GetStateID() override;
};
