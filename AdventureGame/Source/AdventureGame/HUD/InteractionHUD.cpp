// (c) 2025 Sarah Smith


#include "InteractionHUD.h"

#include "../Constants.h"
#include "Components/Image.h"
#include "Internationalization/StringTableRegistry.h"

void UInteractionHUD::SetText(const FText NewText)
{
    if (!TextLocked)
    {
        InteractionDescription->SetText(NewText);
    }
}

void UInteractionHUD::HighlightText()
{
    TextLocked = true;
    InteractionDescription->SetColorAndOpacity(TextHighlightColor);
}

void UInteractionHUD::UnhighlightText()
{
    TextLocked = false;
    InteractionDescription->SetColorAndOpacity(TextNormalColor);
}

void UInteractionHUD::ResetText()
{
    UnhighlightText();
    SetText(LOCTABLE(ITEM_DESCRIPTIONS_KEY, G_WALK_TO_KEY));
}

void UInteractionHUD::FlashTimerTimeout()
{
    if (IsSaveIndicatorFlashOn)
    {
        Bullet->SetColorAndOpacity(G_UI_Dimmed_Colour);
        IsSaveIndicatorFlashOn = false;
    }
    else
    {
        Bullet->SetColorAndOpacity(G_UI_Highlight_Colour);
        IsSaveIndicatorFlashOn = true;
    }
}

void UInteractionHUD::StartSaving()
{
    SaveIndicator->SetText(LOCTABLE(UI_STRINGS_KEY, "SavingGame"));
    IsSaving = true;
    StartFlashing();
}

void UInteractionHUD::EndSaving(bool Success)
{
    SaveIndicator->SetText(Success ? LOCTABLE(UI_STRINGS_KEY, "SavedGame") : LOCTABLE(UI_STRINGS_KEY, "SavedGame"));
    EndFlashing(Success);
}

void UInteractionHUD::StartLoading()
{
    SaveIndicator->SetText(LOCTABLE(UI_STRINGS_KEY, "LoadingGame"));
    StartFlashing();
}

void UInteractionHUD::EndLoading(bool Success)
{
    SaveIndicator->SetText(Success ? LOCTABLE(UI_STRINGS_KEY, "LoadedGame") : LOCTABLE(UI_STRINGS_KEY, "ErrorLoading"));
    EndFlashing(Success);
}

void UInteractionHUD::ReturnToRestTimerTimeout()
{
    Bullet->SetColorAndOpacity(G_UI_Dimmed_Colour);
    SaveIndicator->SetColorAndOpacity(G_UI_Dimmed_Colour);
    IsSaving = false;
}

void UInteractionHUD::StartFlashing()
{
    const UWorld *World = GetWorld();
    if (!World) return;

    // No need to check IsSaving and clear timer here, as the call to SetTimer
    // will clear any currently running timer in this handle.
    World->GetTimerManager().SetTimer(
        FlashTimerHandle, this,
        &UInteractionHUD::FlashTimerTimeout,
        FlashTempo, true);
    Bullet->SetColorAndOpacity(G_UI_Highlight_Colour);
    SaveIndicator->SetColorAndOpacity(G_UI_Highlight_Colour);
    IsSaveIndicatorFlashOn = true;
}

void UInteractionHUD::EndFlashing(bool Success)
{
    const UWorld *World = GetWorld();
    check(World);

    Bullet->SetColorAndOpacity(Success ? G_UI_Status_Color : G_UI_Error_Color);
    SaveIndicator->SetColorAndOpacity(Success ? G_UI_Status_Color : G_UI_Error_Color);
    IsSaveIndicatorFlashOn = false;

    World->GetTimerManager().SetTimer(
        ReturnToRestTimerHandle, this,
        &UInteractionHUD::FlashTimerTimeout,
        ReturnToRestTime, false);
}
