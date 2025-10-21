// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealDiscordRichPresence.h"
#include "TickableEditorObject.h"

#include "discord.h"
#include "DiscordTicker.h"
#define LOCTEXT_NAMESPACE "FUnrealDiscordRichPresenceModule"

void FUnrealDiscordRichPresenceModule::funct(discord::Result result)
{
	UE_LOG(LogTemp, Log, TEXT("Rich presence core create result: %u"), result);
}

void FUnrealDiscordRichPresenceModule::StartupModule()
{
	FWorldDelegates::OnPostWorldInitialization.AddLambda([this](UWorld* World, const UWorld::InitializationValues IVS)
	{
		if (!World) return;

		auto result = discord::Core::Create(1430314763975655465, DiscordCreateFlags_Default, &Core);
		if (result != discord::Result::Ok)
		{
			UE_LOG(LogTemp, Error, TEXT("Discord core creation failed: %d"), (int)result);
			return;
		}

		TickableEditorObject = new FDiscordTicker(Core);

		UE_LOG(LogTemp, Log, TEXT("Discord module started"));
	});
}

void FUnrealDiscordRichPresenceModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealDiscordRichPresenceModule, UnrealDiscordRichPresence)