// Created by Stalker7274

#include "Core/FAssetFocusTracker.h"

FAssetFocusTracker::~FAssetFocusTracker()
{
}

void FAssetFocusTracker::Initialize()
{
	UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
	AssetEditorSubsystem->OnAssetEditorOpened().AddRaw(this, &FAssetFocusTracker::HandleAssetOpened);
        
	FGlobalTabmanager::Get()->OnActiveTabChanged_Subscribe(
		FOnActiveTabChanged::FDelegate::CreateRaw(this, &FAssetFocusTracker::HandleTabChanged)
	);
        
	FEditorDelegates::OnMapOpened.AddRaw(this, &FAssetFocusTracker::HandleMapOpened);
}

void FAssetFocusTracker::Shutdown()
{
	if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
	{
		AssetEditorSubsystem->OnAssetEditorOpened().RemoveAll(this);
	}
        
	FGlobalTabmanager::Get()->OnActiveTabChanged_Unsubscribe(TabChangedHandle);
	FEditorDelegates::OnMapOpened.RemoveAll(this);
}

void FAssetFocusTracker::HandleAssetOpened(UObject* Asset)
{
	if (Asset)
	{
		BroadcastFocus(Asset->GetName());
	}
}

void FAssetFocusTracker::HandleTabChanged(TSharedPtr<SDockTab> PrevTab, TSharedPtr<SDockTab> NewTab)
{
	if (!NewTab.IsValid()) return;

#if ENGINE_MAJOR_VERSION == 5
	TSharedPtr<FTabManager> TabManager = NewTab->GetTabManagerPtr();
#elif ENGINE_MAJOR_VERSION == 4
	TSharedPtr<FTabManager> TabManager = NewTab->GetTabManager();
#endif
	
	if (!TabManager.IsValid()) return;
        
	TSharedPtr<SDockTab> OwnerTab = TabManager->GetOwnerTab();
	if (OwnerTab.IsValid())
	{
		BroadcastFocus(OwnerTab->GetTabLabel().ToString());
	}
}

void FAssetFocusTracker::HandleMapOpened(const FString& Filename, bool bAsTemplate)
{
	FString LevelName = FPaths::GetBaseFilename(Filename);
	BroadcastFocus(LevelName);
}

void FAssetFocusTracker::BroadcastFocus(const FString& AssetName)
{
	if (AssetName != LastAssetName)
	{
		LastAssetName = AssetName;
		OnAssetFocusChanged.ExecuteIfBound(AssetName);
	}
}
