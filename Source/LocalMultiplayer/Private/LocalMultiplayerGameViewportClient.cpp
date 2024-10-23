// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();
	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>())
	{
		if (const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>())
		{
			int PlayerIndex = -1;
			if (EventArgs.IsGamepad())
			{
				if (LocalMultiplayerSubsystem
					->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.InputDevice.GetId()) < 0)
				{
					PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(
						EventArgs.InputDevice.GetId());
					LocalMultiplayerSubsystem
					->AssignGamepadInputMapping(PlayerIndex,ELocalMultiplayerInputMappingType::InGame);
				}
				else
				{
					PlayerIndex = LocalMultiplayerSubsystem
					->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.InputDevice.GetId());
				}

				GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex)->GetPlayerController(GetWorld())
				->InputKey(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad());
			}
			else
			{
				if (int KeyboardProfileIndex = LocalMultiplayerSettings->
					FindKeyboardProfileIndexFromKey(EventArgs.Key,ELocalMultiplayerInputMappingType::InGame); KeyboardProfileIndex != -1)
				{
					if (LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(
						KeyboardProfileIndex) < 0)
					{
						PlayerIndex = LocalMultiplayerSubsystem
						->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
						LocalMultiplayerSubsystem
						->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex,ELocalMultiplayerInputMappingType::InGame);
						
					}
					else
					{
						PlayerIndex = LocalMultiplayerSubsystem
						->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
					}

					GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex)->GetPlayerController(GetWorld())
					->InputKey(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad());
				}
			}
		}
	}
	return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (bGamepad)
	{
		int PlayerIndex = -1;
		if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GetGameInstance()
			->GetSubsystem<ULocalMultiplayerSubsystem>())
		{
			if (!LocalMultiplayerSubsystem->IsGamepadDeviceIDValid(InputDevice.GetId()))
			{
				// Assign a new player to this Gamepad Device ID
				PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
				LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
			}
			else
			{
				// Retrieve the existing player index
				PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
			}

			// Pass input to the player's controller
			GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex)->GetPlayerController(GetWorld())
					->InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}

	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}


