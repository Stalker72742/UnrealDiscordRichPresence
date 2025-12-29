// Created by Stalker7274

#include "UnrealDiscordRichPresence.h"
#include "TickableEditorObject.h"

#include "Core/DiscordTicker.h"
#include "Core/FAssetFocusTracker.h"
#include "Data/PresenceSettings.h"
#include "Data/UnrealPresenceLog.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "FUnrealDiscordRichPresenceModule"

DEFINE_LOG_CATEGORY(LogUnrealPresence);

void FUnrealDiscordRichPresenceModule::RestartPresence(bool bNeedRecreate)
{
	if (bNeedRecreate)
	{
		if (TickableEditorObject && TickableEditorObject->IsConnected())
		{
			delete TickableEditorObject;
			delete FocusTrackerObject;
			
			TickableEditorObject = nullptr;
			FocusTrackerObject = nullptr;

			UE_LOG(LogUnrealPresence, Log, TEXT("Discord presence deleted"));
		}
		
		if (PresenceSettings->bShowPresence)
		{
			TickableEditorObject = new FDiscordTicker();

			FocusTrackerObject = new FAssetFocusTracker();
			FocusTrackerObject->OnAssetFocusChanged.BindLambda([this](const FString& AssetName)
			{
				UE_LOG(LogTemp, Log, TEXT("Focused: %s"), *AssetName);

				if (TickableEditorObject)
				{
					TickableEditorObject->SetState(AssetName);
				}
			});
			FocusTrackerObject->Initialize();

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
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 2
			FWorldDelegates::OnWorldInitializedActors.AddLambda([this](const FActorsInitializedParams& IVS)
			{
				if (TickableEditorObject) return;

				TickableEditorObject = new FDiscordTicker();

				UE_LOG(LogUnrealPresence, Log, TEXT("Discord presence started"));

				FocusTrackerObject = new FAssetFocusTracker();
				FocusTrackerObject->OnAssetFocusChanged.BindLambda([this](const FString& AssetName)
				{
					UE_LOG(LogUnrealPresence, Log, TEXT("Focused: %s"), *AssetName);

					if (TickableEditorObject)
					{
						TickableEditorObject->SetState(AssetName);
					}
				});
				FocusTrackerObject->Initialize();
			});
#elif (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27) || \
	(ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION <= 1)

			FWorldDelegates::OnWorldInitializedActors.AddRaw(this, &FUnrealDiscordRichPresenceModule::OnLevelLoaded);
#endif
		}

		PresenceSettings->OnSettingsChangedDelegate.AddRaw(this, &FUnrealDiscordRichPresenceModule::RestartPresence);
	}else
	{
		UE_LOG(LogUnrealPresence, Error, TEXT("Discord presence settings not valid"));
	}
}

void FUnrealDiscordRichPresenceModule::OnLevelLoaded(const UWorld::FActorsInitializedParams& InParams)
{
	if (TickableEditorObject) return;

	TickableEditorObject = new FDiscordTicker();

	UE_LOG(LogUnrealPresence, Log, TEXT("Discord presence started"));

	FocusTrackerObject = new FAssetFocusTracker();
	FocusTrackerObject->OnAssetFocusChanged.BindLambda([this](const FString& AssetName)
	{
		UE_LOG(LogUnrealPresence, Log, TEXT("Focused: %s"), *AssetName);

		if (TickableEditorObject)
		{
			TickableEditorObject->SetState(AssetName);
		}
	});
	FocusTrackerObject->Initialize();
}

void FUnrealDiscordRichPresenceModule::ShutdownModule()
{
	if (TickableEditorObject)
	{
		delete TickableEditorObject;
	}
	
	if (FocusTrackerObject)
	{
		delete FocusTrackerObject;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealDiscordRichPresenceModule, UnrealDiscordRichPresence)