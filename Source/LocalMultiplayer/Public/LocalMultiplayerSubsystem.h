// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LocalMultiplayerSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSubsystem.generated.h"


class ACouchCharacter;

UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType);

	UFUNCTION(BlueprintCallable)
	TArray<ACouchCharacter*> GetAllPlayers();
	UFUNCTION(BlueprintCallable)
	void AddPlayerToList(ACouchCharacter* Player);

	int GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex);
	
	int AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex);
	
	void AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const;
	
	void SwitchKeyboardMappingType(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const;

	UFUNCTION(BlueprintCallable)
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
	UFUNCTION(BlueprintCallable)
	int GetNbOfAssignedPlayer() const;

	TArray<APlayerController*> GetAllPlayerControllers();

protected:
	TArray<int> AssignedPlayerIndexInOrder;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromKeyboardProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;

private:
	UPROPERTY()
	TArray<TObjectPtr<APlayerController>> PlayerControllers;
	UPROPERTY()
	TArray<ACouchCharacter*> Players;

	
	
	int UnassignPlayerToKeyboardProfile(int KeyboardProfileIndex);
	
	void UnassignKeyboardMapping(int PlayerIndex) const;
	
	void UnassignKeyboardProfile(int PlayerIndex, int KeyboardProfileIndex);
	
	int UnassignPlayerToGamepadDeviceID(int DeviceID);
	
	void UnassignGamepadInputMapping(int PlayerIndex) const;
	
	void UnassignGamepadProfile(int PlayerIndex, int GamepadID);
	
	int GetProfileIndexFromPlayerIndex(int PlayerIndex);
	
	int GetFirstAvailablePlayerIndex() const;
};
