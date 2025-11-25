// Created by Stalker7274

#pragma once

#include "CoreMinimal.h"
#include "DiscordUnreal.h"
#include "UObject/Object.h"

class UDiscordClient;
/**
 * 
 */
class FDiscordTicker : public FTickableEditorObject
{
public:
	FDiscordTicker();
	~FDiscordTicker();

protected:
	
	float TickInterval {0.5f};
	
protected:
	FDiscordUniqueID AppId;

	FString CurrentState;
	FString CurrentDetails;

	int64 StartTimestamp;
	
	float TimeSinceLastUpdate {0.0f};
	
	UDiscordClient* Client;
	UDiscordAuthorizationCodeVerifier* CodeVerifier;

	bool bConnected {false};

protected:

	void OnStatusChanged(EDiscordClientStatus InStatus, EDiscordClientError InError, int32 InErrorDetail);
	void OnUpdateRichPresenceResult(UDiscordClientResult* InResult);
	void OnAuthorization(UDiscordClientResult* InResult, FString InCode, FString InRedirectUri);
	void OnDiscordTokenExchangeResult(UDiscordClientResult* InResult, FString InAccessToken, FString InRefreshToken,
						EDiscordAuthorizationTokenType InTokenType, int32 InExpiresIn, FString InScop);
	void OnUpdateTokenResult(UDiscordClientResult* InResult);

public:

	virtual void Tick(float DeltaTime) override;

	virtual bool IsTickable() const override { return true; }
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(FDiscordTicker, STATGROUP_Tickables); }
	
public:

	virtual void UpdateActivity();
	virtual void SetState(FString InNewState);
	virtual void SetDetails(FString InNewDetails);

public:

	bool IsConnected() const { return bConnected; }
};
