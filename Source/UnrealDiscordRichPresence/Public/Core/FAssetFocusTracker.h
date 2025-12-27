// Created by Stalker7274

#pragma once

DECLARE_DELEGATE_OneParam(FOnAssetFocusChanged, const FString&);

class FAssetFocusTracker
{
public:
    virtual ~FAssetFocusTracker();

    virtual void Initialize();
    virtual void Shutdown();

protected:

    FString LastAssetName;
    FDelegateHandle TabChangedHandle;

protected:
    virtual void HandleAssetOpened(UObject* Asset);
    virtual void HandleTabChanged(TSharedPtr<SDockTab> PrevTab, TSharedPtr<SDockTab> NewTab);
    virtual void HandleMapOpened(const FString& Filename, bool bAsTemplate);
    virtual void BroadcastFocus(const FString& AssetName);

public:
    FOnAssetFocusChanged OnAssetFocusChanged;
};
