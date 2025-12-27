// Created by Stalker7274


#include "Data/PresenceSettings.h"

UPresenceSettings::UPresenceSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPresenceSettings::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	SaveConfig();
	
	if (auto property = PropertyChangedEvent.Property)
	{
		if (property->GetNameCPP() == TEXT("bShowPresence") || property->GetNameCPP() == TEXT("DefaultAppId"))
		{
			OnSettingsChangedDelegate.Broadcast(true);
			return;
		}
	}

	OnSettingsChangedDelegate.Broadcast(false);
}
