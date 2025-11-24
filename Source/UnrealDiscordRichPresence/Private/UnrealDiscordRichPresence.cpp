// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealDiscordRichPresence.h"
#include "TickableEditorObject.h"

#include "Core/DiscordTicker.h"
#include "Data/PresenceSettings.h"

#define LOCTEXT_NAMESPACE "FUnrealDiscordRichPresenceModule"

void FUnrealDiscordRichPresenceModule::RestartPresence(bool bNeedRecreate)
{
	if (bNeedRecreate)
	{
		if (TickableEditorObject && TickableEditorObject->IsConnected())
		{
			delete TickableEditorObject;
			TickableEditorObject = nullptr;
		}
		
		if (PresenceSettings->bShowPresence)
		{
			TickableEditorObject = new FDiscordTicker();

			UE_LOG(LogTemp, Log, TEXT("Discord presence recreated"));
		}
	}
	else
	{
		TickableEditorObject->UpdateActivity();
	}
}

void FUnrealDiscordRichPresenceModule::StartupModule()
{
	PresenceSettings = const_cast<UPresenceSettings*>(GetDefault<UPresenceSettings>());
	
	if (PresenceSettings)
	{
		if (PresenceSettings->bShowPresence)
		{
			FWorldDelegates::OnWorldInitializedActors.AddLambda([this](const FActorsInitializedParams& IVS)
			{
				if (TickableEditorObject) return;

				TickableEditorObject = new FDiscordTicker();

				UE_LOG(LogTemp, Log, TEXT("Discord module started"));
			});
		}

		PresenceSettings->OnSettingsChangedDelegate.AddRaw(this, &FUnrealDiscordRichPresenceModule::RestartPresence);
	}
	
	
}

void FUnrealDiscordRichPresenceModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealDiscordRichPresenceModule, UnrealDiscordRichPresence)