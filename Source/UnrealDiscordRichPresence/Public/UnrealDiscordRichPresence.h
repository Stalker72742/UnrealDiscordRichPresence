// Created by Stalker7274

#pragma once

#include "Modules/ModuleManager.h"

class UPresenceSettings;
class FDiscordTicker;

class FUnrealDiscordRichPresenceModule : public IModuleInterface
{

protected:
	
	UPresenceSettings* PresenceSettings {nullptr};
	
protected:

	void RestartPresence(bool bNeedRecreate);
	
public:
	
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	FDiscordTicker* TickableEditorObject {nullptr};
};
