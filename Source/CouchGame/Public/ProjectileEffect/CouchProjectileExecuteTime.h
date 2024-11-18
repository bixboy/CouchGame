// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CouchProjectileExecuteTime.generated.h"

/**
 * 
 */
UENUM()
enum class ECouchProjectileExecuteTime : uint8
{
	OnImpactWithHit,
	OnImpactWithoutHit,
	OnLaunch,
	OnDelay
};

