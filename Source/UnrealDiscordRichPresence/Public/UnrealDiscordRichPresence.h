// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "types.h"
#include "Modules/ModuleManager.h"
#include "UnrealDiscordRichPresence/DiscordSDK/cpp/core.h"

class FDiscordTicker;

class FUnrealDiscordRichPresenceModule : public IModuleInterface
{
public:

	FTimerHandle timer;

	UFUNCTION()
	void funct(discord::Result result);

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	discord::Core* Core;

	FDiscordTicker* TickableEditorObject;
};
