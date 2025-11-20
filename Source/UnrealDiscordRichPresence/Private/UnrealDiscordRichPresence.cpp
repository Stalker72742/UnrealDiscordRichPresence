// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealDiscordRichPresence.h"
#include "TickableEditorObject.h"

#include "Core/DiscordTicker.h"

#define LOCTEXT_NAMESPACE "FUnrealDiscordRichPresenceModule"

void FUnrealDiscordRichPresenceModule::StartupModule()
{
	FWorldDelegates::OnWorldInitializedActors.AddLambda([this](const FActorsInitializedParams& IVS)
	{
		if (TickableEditorObject) return;

		TickableEditorObject = new FDiscordTicker();

		UE_LOG(LogTemp, Log, TEXT("Discord module started"));
	});
}

void FUnrealDiscordRichPresenceModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealDiscordRichPresenceModule, UnrealDiscordRichPresence)