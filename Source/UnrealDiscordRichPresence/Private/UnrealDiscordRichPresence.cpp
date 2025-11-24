// Created by Stalker7274

#include "UnrealDiscordRichPresence.h"
#include "TickableEditorObject.h"

#include "Core/DiscordTicker.h"
#include "Data/PresenceSettings.h"
#include "Data/UnrealPresenceLog.h"

#define LOCTEXT_NAMESPACE "FUnrealDiscordRichPresenceModule"

void FUnrealDiscordRichPresenceModule::RestartPresence(bool bNeedRecreate)
{
	if (bNeedRecreate)
	{
		if (TickableEditorObject && TickableEditorObject->IsConnected())
		{
			delete TickableEditorObject;
			TickableEditorObject = nullptr;

			UE_LOG(LogUnrealPresence, Log, TEXT("Discord presence deleted"));
		}
		
		if (PresenceSettings->bShowPresence)
		{
			TickableEditorObject = new FDiscordTicker();

			UE_LOG(LogUnrealPresence, Log, TEXT("Discord presence recreated"));
		}
	}
	else
	{
		TickableEditorObject->UpdateActivity();
		
		UE_LOG(LogUnrealPresence, Log, TEXT("Discord presence updated"));
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

				UE_LOG(LogUnrealPresence, Log, TEXT("Discord presence started"));
			});
		}

		PresenceSettings->OnSettingsChangedDelegate.AddRaw(this, &FUnrealDiscordRichPresenceModule::RestartPresence);
	}else
	{
		UE_LOG(LogUnrealPresence, Error, TEXT("Discord presence settings not valid"));
	}
}

void FUnrealDiscordRichPresenceModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealDiscordRichPresenceModule, UnrealDiscordRichPresence)