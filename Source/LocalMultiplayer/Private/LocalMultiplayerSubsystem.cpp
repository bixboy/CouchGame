// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSettings.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Kismet/GameplayStatics.h"



void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	for (int i = 0; i < LocalMultiplayerSettings->NbMaxGamepads + LocalMultiplayerSettings->GetNbKeyboardProfiles(); i++)
	{
		if (!GetWorld()->GetGameInstance()->GetLocalPlayerByIndex(i))
		{
			UGameplayStatics::CreatePlayer(GetWorld(), i);
			// FString PlayerMessage = FString::Printf(TEXT("Player %d created"), i);
			//
			// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, PlayerMessage);
		}

	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if (PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	LastAssignedPlayerIndex++;
	return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (LocalMultiplayerSettings && LocalMultiplayerSettings->KeyboardProfilesData.IsValidIndex(KeyboardProfileIndex))
		{
			const FLocalMultiplayerProfileData& KeyboardProfileData = LocalMultiplayerSettings->KeyboardProfilesData[KeyboardProfileIndex];
			
			if (UInputMappingContext* IMC = KeyboardProfileData.GetIMCFromType(MappingType))
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					FModifyContextOptions ContextOptions;
					ContextOptions.bForceImmediately = true;
					Subsystem->AddMappingContext(IMC, 0, ContextOptions);
				}
			}
		}
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		return PlayerIndexFromGamepadProfileIndex[DeviceID];
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	LastAssignedPlayerIndex++;
	return PlayerIndexFromGamepadProfileIndex[DeviceID];
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (UInputMappingContext* IMC = LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				FModifyContextOptions ContextOptions;
				ContextOptions.bForceImmediately = true;
				Subsystem->AddMappingContext(IMC, 0, ContextOptions);
			}
		}
	}
}

bool ULocalMultiplayerSubsystem::IsGamepadDeviceIDValid(int32 DeviceID) const
{
	return PlayerIndexFromGamepadProfileIndex.Contains(DeviceID);
}






