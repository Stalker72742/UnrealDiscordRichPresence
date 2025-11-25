// Created by Stalker7274

#pragma once

DECLARE_DELEGATE_OneParam(FOnAssetFocusChanged, const FString&);

class FAssetFocusTracker
{
public:
    FOnAssetFocusChanged OnAssetFocusChanged;

    void Initialize()
    {
        UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
        AssetEditorSubsystem->OnAssetEditorOpened().AddRaw(this, &FAssetFocusTracker::HandleAssetOpened);
        
        FGlobalTabmanager::Get()->OnActiveTabChanged_Subscribe(
            FOnActiveTabChanged::FDelegate::CreateRaw(this, &FAssetFocusTracker::HandleTabChanged)
        );
        
        FEditorDelegates::OnMapOpened.AddRaw(this, &FAssetFocusTracker::HandleMapOpened);
    }

    void Shutdown()
    {
        if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
        {
            AssetEditorSubsystem->OnAssetEditorOpened().RemoveAll(this);
        }
        
        FGlobalTabmanager::Get()->OnActiveTabChanged_Unsubscribe(TabChangedHandle);
        FEditorDelegates::OnMapOpened.RemoveAll(this);
    }

private:
    void HandleAssetOpened(UObject* Asset)
    {
        if (Asset)
        {
            BroadcastFocus(Asset->GetName());
        }
    }

    void HandleTabChanged(TSharedPtr<SDockTab> PrevTab, TSharedPtr<SDockTab> NewTab)
    {
        // if (!NewTab.IsValid())
        //     return;
        //
        // // Проверяем что это таб редактора ассета, а не служебная панель
        // FName TabRole = NewTab->GetLayoutIdentifier().TabType;
        //
        // // Табы ассетов обычно имеют роль ETabRole::DocumentTab
        // if (NewTab->GetTabRole() != ETabRole::DocumentTab)
        //     return;
        //
        // FString TabLabel = NewTab->GetTabLabel().ToString();
        // TabLabel.RemoveFromEnd(TEXT("*"));
        // TabLabel.TrimStartAndEndInline();
        //
        // if (!TabLabel.IsEmpty() && TabLabel != LastAssetName)
        // {
        //     BroadcastFocus(TabLabel);
        // }

        if (!NewTab.IsValid()) return;

        TSharedPtr<FTabManager> TabManager = NewTab->GetTabManagerPtr();
        if (!TabManager.IsValid()) return;

        // Пробуем получить owner window и через него Toolkit
        TSharedPtr<SDockTab> OwnerTab = TabManager->GetOwnerTab();
        if (OwnerTab.IsValid())
        {
            BroadcastFocus(OwnerTab->GetTabLabel().ToString());
        }
    }

    void HandleMapOpened(const FString& Filename, bool bAsTemplate)
    {
        FString LevelName = FPaths::GetBaseFilename(Filename);
        BroadcastFocus(LevelName);
    }

    void BroadcastFocus(const FString& AssetName)
    {
        if (AssetName != LastAssetName)
        {
            LastAssetName = AssetName;
            OnAssetFocusChanged.ExecuteIfBound(AssetName);
        }
    }

    FString LastAssetName;
    FDelegateHandle TabChangedHandle;
};
