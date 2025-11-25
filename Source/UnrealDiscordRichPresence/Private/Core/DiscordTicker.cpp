// Created by Stalker7274


#include "Core/DiscordTicker.h"

#include "DiscordUnreal.h"
#include "Data/PresenceSettings.h"
#include "Data/UnrealPresenceLog.h"

FDiscordTicker::FDiscordTicker() 
{
	CurrentDetails = FApp::GetProjectName();
	CurrentState = "Idle";

	StartTimestamp = FDateTime::UtcNow().ToUnixTimestamp();

	if (auto presenceSettings = GetDefault<UPresenceSettings>())
	{
		AppId = FDiscordUniqueID(FCString::Atoi64(*presenceSettings->DefaultAppId));
	}
	else
	{
		UE_LOG(LogUnrealPresence, Error, TEXT("No presence settings found"));
		return;
	}

	Client = NewObject<UDiscordClient>();
	Client->AddToRoot();
	Client->Init();

	Client->SetStatusChangedCallback(FDiscordClientOnStatusChanged::CreateRaw(this, &FDiscordTicker::OnStatusChanged));

	CodeVerifier = Client->CreateAuthorizationCodeVerifier();
	auto AuthArgs = NewObject<UDiscordAuthorizationArgs>();
	AuthArgs->Init();
	AuthArgs->SetClientId(AppId);
	AuthArgs->SetScopes(UDiscordClient::GetDefaultPresenceScopes());
	AuthArgs->SetCodeChallenge(CodeVerifier->Challenge());

	//Todo make config path and name in var 
	FString SavedRefreshToken;
	GConfig->GetString(TEXT("/Script/Engine.GameUserSettings"), TEXT("DiscordRefreshToken"), SavedRefreshToken, GGameUserSettingsIni);
	
	if (!SavedRefreshToken.IsEmpty())
	{
		UE_LOG(LogUnrealPresence, Log, TEXT("Found token. Trying refresh token"));
		
		Client->RefreshToken(AppId, SavedRefreshToken, 
			FDiscordClientTokenExchangeCallback::CreateRaw(this, &FDiscordTicker::OnDiscordTokenExchangeResult));
	}
	else
	{
		UE_LOG(LogUnrealPresence, Log, TEXT("Fail to find token. Authorizing..."));
		
		Client->Authorize(AuthArgs, FDiscordClientAuthorizationCallback::CreateRaw(this, &FDiscordTicker::OnAuthorization));
	}
}

FDiscordTicker::~FDiscordTicker()
{
	UE_LOG(LogUnrealPresence, Log, TEXT("Destroying discord presence"));

	// Crash without !IsEngineExitRequested()
	if (Client && IsValid(Client) && !IsEngineExitRequested())
	{
		if (bConnected)
		{
			if (!IsValid(Client))
			{
				Client->Disconnect();
			}
			bConnected = false;
		}
        
		Client->RemoveFromRoot();
		Client->MarkAsGarbage();
		Client = nullptr;
	}
}

void FDiscordTicker::OnDiscordTokenExchangeResult(UDiscordClientResult* InResult, FString InAccessToken,
                                                  FString InRefreshToken, EDiscordAuthorizationTokenType InTokenType, int32 InExpiresIn, FString InScop)
{
	if (!InResult->Successful()) {
		UE_LOG(LogUnrealPresence, Error, TEXT("Discord token exchange failed: %s"), *InResult->Error());
	}

	if (!InRefreshToken.IsEmpty())
	{
		//Todo make config path and name in var 
		GConfig->SetString(TEXT("/Script/Engine.GameUserSettings"), TEXT("DiscordRefreshToken"), *InRefreshToken, GGameUserSettingsIni);
		GConfig->Flush(false, GGameUserSettingsIni);
	}

	Client->UpdateToken(InTokenType, InAccessToken, FDiscordClientUpdateTokenCallback::CreateRaw(this, &FDiscordTicker::OnUpdateTokenResult));
}

void FDiscordTicker::OnUpdateTokenResult(UDiscordClientResult* InResult)
{
	Client->Connect();
	bConnected = true;
	
	UE_LOG(LogUnrealPresence, Log, TEXT("Presence connected to discord client")); 
}

void FDiscordTicker::OnAuthorization(UDiscordClientResult* InResult, FString InCode, FString InRedirectUri)
{
	if (!InResult->Successful()) {
		UE_LOG(LogUnrealPresence, Error, TEXT("Discord authorization failed: %s"), *InResult->Error());
		return;
	}

	UE_LOG(LogUnrealPresence, Log, TEXT("Authorization successful! Getting access token..."));
	Client->GetToken(AppId, InCode,  CodeVerifier->Verifier(), InRedirectUri,
		FDiscordClientTokenExchangeCallback::CreateRaw(this, &FDiscordTicker::OnDiscordTokenExchangeResult));
}

void FDiscordTicker::Tick(float DeltaTime)
{
	Discord_RunCallbacks();
}

void FDiscordTicker::UpdateActivity()
{
	UDiscordActivity* Activity = NewObject<UDiscordActivity>();
	Activity->Init();
	Activity->SetType(EDiscordActivityTypes::Playing);
	Activity->AddToRoot();
	
	if (auto presenceSettings = GetDefault<UPresenceSettings>())
	{
		if (presenceSettings->bUseCustomDetails)
		{
			Activity->SetDetails(presenceSettings->CustomDetails);
		}
		else
		{
			Activity->SetDetails(CurrentDetails);
		}

		if (presenceSettings->bUseCustomState)
		{
			Activity->SetState(presenceSettings->CustomState);
		}
		else
		{
			Activity->SetState(CurrentState);
		}
	}
	else
	{
		Activity->SetDetails(CurrentDetails);
		Activity->SetState(CurrentState);
	}
	
	UDiscordActivityTimestamps* Timestamps = NewObject<UDiscordActivityTimestamps>();
	Timestamps->Init();

	Timestamps->SetStart(StartTimestamp);
	
	Activity->SetTimestamps(Timestamps);

	Client->UpdateRichPresence(Activity, FDiscordClientUpdateRichPresenceCallback::CreateRaw(
		this, &FDiscordTicker::OnUpdateRichPresenceResult));
}

void FDiscordTicker::OnStatusChanged(EDiscordClientStatus Status, EDiscordClientError Error, int32 ErrorDetail) {
	
	if (Status == EDiscordClientStatus::Ready) {
		UE_LOG(LogUnrealPresence, Log, TEXT("Connected to Discord! Ready to go! You can now start using Discord features."));

		UpdateActivity();
	}
	else if (Error != EDiscordClientError::None) {
		UE_LOG(LogUnrealPresence, Log, TEXT("Connection error: %s (Detail: %d)"), *UEnum::GetValueAsString(Error), ErrorDetail);
		bConnected = false; 
	}
	else
	{
		UE_LOG(LogUnrealPresence, Log, TEXT("Connection status: %s"), *UEnum::GetValueAsString(Status));
	}
}

void FDiscordTicker::OnUpdateRichPresenceResult(UDiscordClientResult* InResult)
{
	if (InResult->Successful()) {
		UE_LOG(LogUnrealPresence, Log, TEXT("Rich Presence updated successfully!"));
	} else {
		UE_LOG(LogUnrealPresence, Error, TEXT("Rich Presence update failed"));
	}
}

void FDiscordTicker::SetState(FString InNewState)
{
	CurrentState = InNewState;
	UpdateActivity();
}

void FDiscordTicker::SetDetails(FString InNewDetails)
{
	CurrentDetails = InNewDetails;
	UpdateActivity();
}
