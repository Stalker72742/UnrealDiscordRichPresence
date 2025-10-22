// Fill out your copyright notice in the Description page of Project Settings.


#include "DiscordTicker.h"

void FDiscordTicker::Tick(float DeltaTime)
{
	if (!Core) return;

	Core->RunCallbacks();

	Accum += DeltaTime;
	if (Accum >= 1.f)
	{
		Accum = 0.f;
		
		discord::Activity activity{};
		activity.SetState("Coding");
		activity.SetDetails("Creating bugs");
		
		activity.GetAssets().SetLargeImage("frame_14");
		activity.GetAssets().SetSmallImage("frame_14");

		Core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
			UE_LOG(LogTemp, Log, TEXT("Discord update result: %u"), (uint32)result);
		});
	}
}
