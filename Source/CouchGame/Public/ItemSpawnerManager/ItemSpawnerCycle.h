// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemSpawnerCycle.generated.h"

UENUM()
enum class EItemSpawnerCycle : uint8
{
	Loop,
	PingPong,
	Random,
};
