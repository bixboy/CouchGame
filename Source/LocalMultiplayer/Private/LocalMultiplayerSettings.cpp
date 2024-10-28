// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"

#include "EnhancedActionKeyMapping.h"
#include "InputMappingContext.h"

bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	UInputMappingContext* IMC = GetIMCFromType(MappingType);
	if (IMC)
	{
		for (const FEnhancedActionKeyMapping& Mapping : IMC->GetMappings())
		{
			if (Mapping.Key == Key)
			{
				return true;
			}
		}
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
		case ELocalMultiplayerInputMappingType::InGame:
			return IMCInGame;
		case ELocalMultiplayerInputMappingType::Menu:
			return IMCInMenu;
		default :
			return nullptr;
	}
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return KeyboardProfilesData.Num();
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < KeyboardProfilesData.Num(); i++)
	{
		if (KeyboardProfilesData[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}
	
	return -1;
}
