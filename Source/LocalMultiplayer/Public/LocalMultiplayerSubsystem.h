// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocalMultiplayerSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSubsystem.generated.h"


UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType);

	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);
	
	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);
	
	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const;
	
	void SwitchKeyboardMappingType(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const;

	int GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID);

	int AssignNewPlayerToGamepadDeviceID(int DeviceID);
	
	void AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;
	
	void SwitchGamepadInputMappingType(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const;

	UFUNCTION(BlueprintCallable)
	void UnassignProfile(int ProfileIndex);
	
	bool PlayerHasTheRelevantIMC(int PlayerIndex, ELocalMultiplayerInputMappingType , int KeyboardProfileIndex = -1) const;
	
	bool IsGamepadDeviceIDValid(int32 DeviceID) const;
	
	UFUNCTION(BlueprintCallable)
	int GetLastAssignedPlayerIndex() const;

protected:
	TArray<int> AssignedPlayerIndexInOrder;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;

private:
	int UnassignPlayerToKeyboardProfile(int KeyboardProfileIndex);
	
	void UnassignKeyboardMapping(int PlayerIndex) const;
	
	void UnassignKeyboardProfile(int PlayerIndex, int KeyboardProfileIndex);
	
	int UnassignPlayerToGamepadDeviceID(int DeviceID);
	
	void UnassignGamepadInputMapping(int PlayerIndex) const;
	
	void UnassignGamepadProfile(int PlayerIndex, int GamepadID);
	
	int GetProfileIndexFromPlayerIndex(int PlayerIndex);
	
	int GetFirstAvailablePlayerIndex() const;
};
