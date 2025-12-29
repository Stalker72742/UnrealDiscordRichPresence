// Created by Stalker7274

#pragma once

#include "Core/FAssetFocusTracker.h"
#include "Modules/ModuleManager.h"

class UPresenceSettings;
class FDiscordTicker;

class FUnrealDiscordRichPresenceModule : public IModuleInterface
{

protected:
	
	UPresenceSettings* PresenceSettings {nullptr};

protected:

#if ENGINE_MAJOR_VERSION == 5
	
	UFUNCTION()
	void OnLevelLoaded(const UWorld::FActorsInitializedParams& InParams);
#elif ENGINE_MAJOR_VERSION == 4
	UFUNCTION()
	void OnLevelLoaded(const UWorld::FActorsInitializedParams& InParams);
#endif
	
protected:

	void RestartPresence(bool bNeedRecreate);
	
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FDiscordTicker* TickableEditorObject {nullptr};
	FAssetFocusTracker* FocusTrackerObject {nullptr};
};
