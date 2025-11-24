// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/PresenceSettings.h"

UPresenceSettings::UPresenceSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPresenceSettings::UpdatePresence()
{
	SaveConfig();
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
