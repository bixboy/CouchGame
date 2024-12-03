// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSettings.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	for (int i = 0; i < LocalMultiplayerSettings->NbMaxGamepads + LocalMultiplayerSettings->GetNbKeyboardProfiles(); i++)
	{
		if (!GetWorld()->GetGameInstance()->GetLocalPlayerByIndex(i))
		{
			PlayerControllers.Add(UGameplayStatics::CreatePlayer(GetWorld(), i));
		}

	}
}

TArray<ACouchCharacter*> ULocalMultiplayerSubsystem::GetAllPlayers()
{
	return Players;
}

void ULocalMultiplayerSubsystem::AddPlayerToList(ACouchCharacter* Player)
{
	if (!Players.Contains(Player)) Players.Add(Player);
}

#pragma region keyboard
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
	if (int PlayerIndex = GetFirstAvailablePlayerIndex(); PlayerIndex != -1)
	{
		PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, PlayerIndex);
		AssignedPlayerIndexInOrder.Add(PlayerIndex);
		return PlayerIndex;
	}
	return -1;
	
}
int ULocalMultiplayerSubsystem::UnassignPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	if (PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Unassigned Player To Keyboard Profile");
		int RemovedPlayerIndex = PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
		PlayerIndexFromKeyboardProfileIndex.Remove(KeyboardProfileIndex);
		AssignedPlayerIndexInOrder.Remove(RemovedPlayerIndex);
		return RemovedPlayerIndex;
	}
	return -1;
}

TArray<APlayerController*> ULocalMultiplayerSubsystem::GetAllPlayerControllers()
{
	return PlayerControllers;
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

void ULocalMultiplayerSubsystem::UnassignKeyboardMapping(int PlayerIndex) const
{
	if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();
		}
	}
}

void ULocalMultiplayerSubsystem::UnassignKeyboardProfile(int PlayerIndex, int KeyboardProfileIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Unassigned Keyboard Profile");
	UnassignKeyboardMapping(PlayerIndex);
	UnassignPlayerToKeyboardProfile(KeyboardProfileIndex);
}

void ULocalMultiplayerSubsystem::SwitchKeyboardMappingType(int PlayerIndex, int KeyboardProfileIndex,
                                                           ELocalMultiplayerInputMappingType MappingType) const
{
	if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();
			AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, MappingType);
		}
	}
	
}

#pragma endregion
#pragma region Gamepad
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
	if (int PlayerIndex = GetFirstAvailablePlayerIndex(); PlayerIndex != -1)
	{
		PlayerIndexFromGamepadProfileIndex.Add(DeviceID, PlayerIndex);
		AssignedPlayerIndexInOrder.Add(PlayerIndex);
		return PlayerIndex;
	}
	return -1;
}

int ULocalMultiplayerSubsystem::UnassignPlayerToGamepadDeviceID(int DeviceID)
{
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Unassigned Player To Gamepad Device");
		int RemovedPlayerIndex = PlayerIndexFromGamepadProfileIndex[DeviceID];
		PlayerIndexFromGamepadProfileIndex.Remove(DeviceID);
		AssignedPlayerIndexInOrder.Remove(RemovedPlayerIndex);
		return RemovedPlayerIndex;
	}
	return -1;
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

void ULocalMultiplayerSubsystem::UnassignGamepadInputMapping(int PlayerIndex) const
{
	if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();
		}
	}
}

void ULocalMultiplayerSubsystem::UnassignGamepadProfile(int PlayerIndex, int DeviceID)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Unassigned Gamepad Profile");
	UnassignGamepadInputMapping(PlayerIndex);
	UnassignPlayerToGamepadDeviceID(DeviceID);
}

void ULocalMultiplayerSubsystem::SwitchGamepadInputMappingType(int PlayerIndex,
ELocalMultiplayerInputMappingType MappingType) const
{
	if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->ClearAllMappings();
			AssignGamepadInputMapping(PlayerIndex, MappingType);
		}
		
	}
}

#pragma endregion
void ULocalMultiplayerSubsystem::UnassignProfile(int PlayerIndex)
{
	if (PlayerIndex == -1) return;
	
	if (const int ProfileIndex = GetProfileIndexFromPlayerIndex(PlayerIndex); ProfileIndex != -1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f,FColor::Red, FString::Printf(TEXT("Unassigned Profile Index: %d"), ProfileIndex)
);

		if (GetAssignedPlayerIndexFromGamepadDeviceID(ProfileIndex) == PlayerIndex)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, "A");
			UnassignGamepadProfile(PlayerIndex,ProfileIndex);
		}
		else if (GetAssignedPlayerIndexFromKeyboardProfileIndex(ProfileIndex) == PlayerIndex)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, "B");
			UnassignKeyboardProfile(PlayerIndex, ProfileIndex);
		}
	}
}

bool ULocalMultiplayerSubsystem::PlayerHasTheRelevantIMC(int PlayerIndex,
ELocalMultiplayerInputMappingType MappingType, int KeyboardProfileIndex) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	if (ULocalPlayer* LocalPlayer = GetGameInstance()->GetLocalPlayerByIndex(PlayerIndex))
	{
		UInputMappingContext* IMC;
		if (KeyboardProfileIndex == -1)
		{
			IMC = LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType);
		}
		else
		{
			IMC = LocalMultiplayerSettings->KeyboardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType);
		}
		if (IMC)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				return Subsystem->HasMappingContext(IMC);
			}
		}
	}
	return false;
}


bool ULocalMultiplayerSubsystem::IsGamepadDeviceIDValid(int32 DeviceID) const
{
	return PlayerIndexFromGamepadProfileIndex.Contains(DeviceID);
}

int ULocalMultiplayerSubsystem::GetLastAssignedPlayerIndex() const
{
	if (AssignedPlayerIndexInOrder.Num() > 0)
	{
		return AssignedPlayerIndexInOrder.Last();
	}
	return -1; 
}

int ULocalMultiplayerSubsystem::GetNbOfAssignedPlayer() const
{
	//return AssignedPlayerIndexInOrder.Num();
	return PlayerIndexFromGamepadProfileIndex.Num() + PlayerIndexFromKeyboardProfileIndex.Num();
}

int ULocalMultiplayerSubsystem::GetProfileIndexFromPlayerIndex(int PlayerIndex)
{
	for (const auto Entry : PlayerIndexFromKeyboardProfileIndex)
	{
		if (Entry.Value == PlayerIndex)
		{
			return Entry.Key;
		}
	}
	for (const auto Entry : PlayerIndexFromGamepadProfileIndex)
	{
		if (Entry.Value == PlayerIndex)
		{
			return Entry.Key;
		}
	}
	return -1;
}


int ULocalMultiplayerSubsystem::GetFirstAvailablePlayerIndex() const
{
	TSet AvailablePlayerIndexes = {0, 1, 2, 3};
	for (const TTuple<int, int>& KeyboardProfile : PlayerIndexFromKeyboardProfileIndex)
	{
		AvailablePlayerIndexes.Remove(KeyboardProfile.Value);
	}
	for (const TTuple<int, int>& GamepadProfile : PlayerIndexFromGamepadProfileIndex)
	{
		AvailablePlayerIndexes.Remove(GamepadProfile.Value);
	}
	if (AvailablePlayerIndexes.Num() > 0)
	{
		return *AvailablePlayerIndexes.begin();
	}
	return -1;
}







