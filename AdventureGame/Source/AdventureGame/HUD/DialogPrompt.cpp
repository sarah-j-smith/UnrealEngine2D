// (c) 2025 Sarah Smith


#include "DialogPrompt.h"

#include "AdventureGame/Constants.h"
#include "AdventureGame/Player/AdventurePlayerController.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UDialogPrompt::NativeOnInitialized()
{
    PromptButton->OnClicked.AddUniqueDynamic(this, &UDialogPrompt::HandleOnClicked);
    PromptButton->OnHovered.AddDynamic(this, &UDialogPrompt::HandleOnHover);
    PromptButton->OnUnhovered.AddDynamic(this, &UDialogPrompt::HandleOnUnhover);
}

void UDialogPrompt::SetText(FText TextToSet)
{
    SetVisibility(ESlateVisibility::Visible);
    PromptText->SetText(TextToSet);
}

void UDialogPrompt::HighlightText()
{
    PromptText->SetColorAndOpacity(G_UI_Highlight_Colour);
    PromptBullet->SetBrushTintColor(G_UI_Highlight_Colour);
}

void UDialogPrompt::ResetText()
{
    PromptText->SetColorAndOpacity(HasBeenUsed ? G_UI_Disabled_Colour : G_UI_Dimmed_Colour);
    PromptBullet->SetBrushTintColor(HasBeenUsed ? G_UI_Disabled_Colour : G_UI_Dimmed_Colour);
}

void UDialogPrompt::HidePrompt()
{
    PromptText->SetText(FText::GetEmpty());
    SetVisibility(ESlateVisibility::Hidden);
}

void UDialogPrompt::HandleOnClicked()
{
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController))
    {
        if (IsVisible())
        {
            // AdventurePlayerController->HandleInventoryItemClicked(this);
        }
    }
}

void UDialogPrompt::HandleOnHover()
{
    HighlightText();
}

void UDialogPrompt::HandleOnUnhover()
{
    ResetText();
}

