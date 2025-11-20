// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "PresenceSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, DisplayName="Discrord presence")
class UNREALDISCORDRICHPRESENCE_API UPresenceSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	UPresenceSettings(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, Config, Category="Presence settings")
	bool bShowPresence {false};

	UPROPERTY(EditAnywhere, Config, meta=(EditCondition="bShowPresence"), Category="customizing")
	FString DefaultAppId {TEXT("1430593506191347843")};

public:

	virtual FName GetCategoryName() const override { return TEXT("Plugins"); }
};
