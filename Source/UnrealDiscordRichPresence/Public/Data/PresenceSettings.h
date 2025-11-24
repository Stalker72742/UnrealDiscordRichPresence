// Created by Stalker7274

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PresenceSettings.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPresenceSettingsChanged, bool);

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, DisplayName="Discrord presence")
class UNREALDISCORDRICHPRESENCE_API UPresenceSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	UPresenceSettings(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(CallInEditor)
	void UpdatePresence();
	
	UPROPERTY(EditAnywhere, Config, Category="Presence settings")
	bool bShowPresence {false};

	UPROPERTY(EditAnywhere, Config, meta=(EditCondition="bShowPresence"), Category="Customizing")
	FString DefaultAppId {TEXT("1430593506191347843")};

	UPROPERTY(EditAnywhere, Config, meta=(EditCondition="bShowPresence"), Category="Customizing")
	bool bUseCustomDetails {false};

	UPROPERTY(EditAnywhere, Config, meta=(EditCondition="bShowPresence && bUseCustomDetails"), Category="Customizing")
	FString CustomDetails {TEXT("")};

	UPROPERTY(EditAnywhere, Config, meta=(EditCondition="bShowPresence"), Category="Customizing")
	bool bUseCustomState {false};

	UPROPERTY(EditAnywhere, Config, meta=(EditCondition="bShowPresence && bUseCustomState"), Category="Customizing")
	FString CustomState {TEXT("")};

public:

	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

public:

	FOnPresenceSettingsChanged OnSettingsChangedDelegate;
};
