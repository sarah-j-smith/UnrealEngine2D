// (c) 2025 Sarah Smith


#include "AdventureGameHUD.h"

#include "../Constants.h"
#include "../AdventureGame.h"
#include "../Player/AdventureCharacter.h"
#include "../Player/AdventurePlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "AdvGameUtils.h"

void UAdventureGameHUD::NativeOnInitialized()
{
    PlayerCharacter = GetOwningPlayerPawn<AAdventureCharacter>();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (AAdventurePlayerController* APC = Cast<AAdventurePlayerController>(PlayerController))
    {
        APC->BeginActionDelegate.AddUObject(this, &UAdventureGameHUD::BeginActionEvent);
        APC->UpdateInteractionTextDelegate.AddUObject(
            this, &UAdventureGameHUD::UpdateInteractionTextEvent);
        APC->InterruptActionDelegate.AddUObject(this, &UAdventureGameHUD::InterruptActionEvent);
        APC->UpdateInventoryTextDelegate.AddUObject(
            this, &UAdventureGameHUD::UpdateInventoryTextEvent);
        AdventurePlayerController = APC;
    }

    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameHUD::NativeOnInitialized"));
}

void UAdventureGameHUD::ShowBlackScreen()
{
    BlackScreen->SetVisibility(ESlateVisibility::Visible);
}

void UAdventureGameHUD::HideBlackScreen()
{
    BlackScreen->SetVisibility(ESlateVisibility::Hidden);
}

void UAdventureGameHUD::SetInteractionText()
{
    if (!AdventurePlayerController.IsValid()) return;
    auto Verb = AdventurePlayerController->CurrentVerb;
    const UInventoryItem* SourceItem = AdventurePlayerController->SourceItem;
    if ((Verb == EVerbType::GiveItem || Verb == EVerbType::UseItem) && SourceItem == nullptr)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("Tried to %s with no source item"), *VerbGetDescriptiveString(Verb).ToString());
        return;
    }
    if (auto CurrentHotspot = AdventurePlayerController->CurrentHotSpot)
    {
        FText InteractionText;
        switch (Verb)
        {
        case EVerbType::UseItem:
            InteractionText = AdvGameUtils::GetUsingItemText(SourceItem, nullptr, CurrentHotspot);
            break;
        case EVerbType::Give:
            // Can't just give a hotspot - have to give _something_ to the hotspot - eg GiveItem
            InteractionText = LOCTABLE(ITEM_STRINGS_KEY, "GiveDefaultText");
            break;
        case EVerbType::GiveItem:
            InteractionText = AdvGameUtils::GetGivingItemText(SourceItem, nullptr, CurrentHotspot);
            break;
        default:
            InteractionText = AdvGameUtils::GetVerbWithHotSpotText(CurrentHotspot, Verb);
        }
        InteractionUI->SetText(InteractionText);
        UE_LOG(LogAdventureGame, Log, TEXT("Set interaction text to: %s"), *InteractionText.ToString());
        if (AdventurePlayerController->ShouldHighlightInteractionText())
        {
            InteractionUI->HighlightText();
        }
    }
    else
    {
        FText VerbStr = VerbGetDescriptiveString(Verb);
        InteractionUI->SetText(VerbStr);
    }
}

void UAdventureGameHUD::SetInventoryText()
{
    if (!AdventurePlayerController.IsValid()) return;
    const EVerbType Verb = AdventurePlayerController->CurrentVerb;
    FText InventoryText;
    const UInventoryItem* SourceItem = AdventurePlayerController->SourceItem;
    const UInventoryItem* TargetItem = AdventurePlayerController->TargetItem;
    const AHotSpot* HotSpot = AdventurePlayerController->CurrentHotSpot;
    if (SourceItem == nullptr)
    {
        InteractionUI->ResetText();
        return;
    }
    switch (Verb)
    {
    case EVerbType::Use:
    case EVerbType::UseItem:
        if (SourceItem == TargetItem) return;
        InventoryText = (TargetItem || HotSpot)
                            ? AdvGameUtils::GetUsingItemText(SourceItem, TargetItem, HotSpot)
                            : AdvGameUtils::GetVerbWithItemText(SourceItem, Verb);
        break;
    case EVerbType::GiveItem:
    case EVerbType::Give:
        if (SourceItem == TargetItem) return;
        InventoryText = (TargetItem || HotSpot)
                            ? AdvGameUtils::GetGivingItemText(SourceItem, TargetItem, HotSpot)
                            : AdvGameUtils::GetVerbWithItemText(SourceItem, Verb);
        break;
    default:
        InventoryText = AdvGameUtils::GetVerbWithItemText(SourceItem, Verb);
    }
    InteractionUI->SetText(InventoryText);
    if (AdventurePlayerController->ShouldHighlightInteractionText())
    {
        InteractionUI->HighlightText();
    }
}

void UAdventureGameHUD::ShowPromptList()
{
    UISwitcher->SetActiveWidget(PromptList);
}

void UAdventureGameHUD::HidePromptList()
{
}

void UAdventureGameHUD::AddBarkText(const FText& BarkText, USphereComponent* Position, TOptional<FColor> TextColor)
{
    Bark->BarkTextColor = TextColor.Get(FColor::White);
    Bark->BarkPosition = Position;
    Bark->SetText(BarkText);
}

void UAdventureGameHUD::ClearBarkText()
{
}

void UAdventureGameHUD::HandleInventoryChanged(FName Identifier)
{
    if (Identifier == "Inventory")
    {
        InventoryUI->PopulateInventory(true);
    }
}

void UAdventureGameHUD::UpdateInteractionTextEvent()
{
    SetInteractionText();
}

void UAdventureGameHUD::UpdateInventoryTextEvent()
{
    SetInventoryText();
}

void UAdventureGameHUD::BeginActionEvent()
{
    InteractionUI->HighlightText();
}

void UAdventureGameHUD::InterruptActionEvent()
{
    InteractionUI->ResetText();
    VerbsUI->ClearActiveButton();
}

void UAdventureGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    bool MouseIsOverUI = IsHovered();
    AdventurePlayerController->SetMouseOverUI(MouseIsOverUI);
}
