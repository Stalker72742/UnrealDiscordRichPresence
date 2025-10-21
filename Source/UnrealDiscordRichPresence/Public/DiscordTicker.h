// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnrealDiscordRichPresence/DiscordSDK/cpp/core.h"
#include "UObject/Object.h"

/**
 * 
 */
class FDiscordTicker : public FTickableEditorObject
{
public:
	FDiscordTicker(discord::Core* InCore)
		: Core(InCore), Accum(0.f)
	{
		
	}

	virtual bool IsTickable() const override { return true; }

	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }

	virtual void Tick(float DeltaTime) override
	{
		if (!Core) return;

		Core->RunCallbacks();

		Accum += DeltaTime;
		if (Accum >= 15.f)
		{
			Accum = 0.f;

			discord::Activity activity{};
			activity.SetState("Testing");
			activity.SetDetails("Fruit Loops");

			Core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
				UE_LOG(LogTemp, Log, TEXT("Discord update result: %u"), (uint32)result);
			});
		}
	}

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FDiscordTicker, STATGROUP_Tickables);
	}

private:
	TSharedPtr<discord::Core> Core;
	float Accum;
};
