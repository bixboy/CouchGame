// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"


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
            int PlayerIndex;
        	
            if (EventArgs.IsGamepad())
            {
                int GamepadDeviceID = EventArgs.InputDevice.GetId();
            	PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(GamepadDeviceID);
                if (PlayerIndex < 0)
                {
                    PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(GamepadDeviceID);
                    LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
                }
                else
                {
                    PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(GamepadDeviceID);
                }
                
                if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
                {
                    if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
                    {
                        PlayerController->InputKey(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad());
                    }
                }
            }
            else 
            {
                if (int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame); KeyboardProfileIndex != -1)
                {
                    if (LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex) < 0)
                    {
                        PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
                        LocalMultiplayerSubsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
                    }
                    else
                    {
                        PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
                    }
                    if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
                    {
                        if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
                        {
                            PlayerController->InputKey(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad());
                        }
                    }
                }
            }
        }
    }

    return Super::InputKey(EventArgs);
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (!bGamepad) return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);

	int32 DeviceID = InputDevice.GetId();

	if (ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GetGameInstance()->GetSubsystem<ULocalMultiplayerSubsystem>())
	{
		int32 PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
		if (PlayerIndex < 0)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(DeviceID);
			LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
		else
		{
			PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
		}
		
		if (PlayerIndex != -1)
		{
			if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
			{
				if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
				{
					PlayerController->InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
				}
			}
		}
	}
	
	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}




