// (c) 2025 Sarah Smith


#include "InventoryItem.h"

#include "../Constants.h"
#include "../AdventureGame.h"
#include "../Player/AdventurePlayerController.h"

#include "Internationalization/StringTableRegistry.h"

//////////////////////////////////
///
/// STATIC IMPLEMENTATIONS
///

void UInventoryItem::OnItemCombineSuccess_Implementation()
{
    UE_LOG(LogAdventureGame, Log, TEXT("OnItemCombineSuccess Success - default."));
}

void UInventoryItem::OnItemCombineFailure_Implementation()
{
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemUsedDefaultText"));
}

void UInventoryItem::CombineWithInteractableItem(EItemKind ResultingItem, FText BarkText)
{
    if (const auto Apc = this->AdventurePlayerController.Pin())
    {
        if (Apc->SourceItem)
        {
            Apc->CombineItems(
                Apc->SourceItem,
                this, ResultingItem,
                BarkText.IsEmpty() ? LOCTABLE(ITEM_STRINGS_KEY, "CombineSuccessResultText") : BarkText);
            return;
        }
    }
    UE_LOG(LogAdventureGame, Warning,
        TEXT("Could not CombineWithInteractableItem: AdventurePlayerController->CurrentItem null."));
}

void UInventoryItem::OnItemGiveSuccess_Implementation()
{
    UE_LOG(LogAdventureGame, Log, TEXT("OnItemGiveSuccess Success - default."));
}

void UInventoryItem::OnItemGiveFailure_Implementation()
{
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemGivenDefaultText"));
}

void UInventoryItem::OnClose_Implementation()
{
    IVerbInteractions::OnClose_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On close"));
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "CloseDefaultText"));
}

void UInventoryItem::OnOpen_Implementation()
{
    IVerbInteractions::OnOpen_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On open"));
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "OpenDefaultText"));
}

void UInventoryItem::OnGive_Implementation()
{
    IVerbInteractions::OnGive_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On give inventory item defaultasdfasdfasdf"));
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "GiveDefaultText"));
}

void UInventoryItem::OnPickUp_Implementation()
{
    IVerbInteractions::OnPickUp_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On Pickup"));
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "PickUpDefaultText"));
}

void UInventoryItem::OnTalkTo_Implementation()
{
    IVerbInteractions::OnTalkTo_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On talk"));
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "TalkToDefaultText"));
}

void UInventoryItem::OnLookAt_Implementation()
{
    IVerbInteractions::OnLookAt_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On look at"));
    if (Description.IsEmpty())
    {
        BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "LookAtDefaultText"));
    }
    else
    {
        BarkAndEnd(Description);
    }
}

void UInventoryItem::OnPull_Implementation()
{
    IVerbInteractions::OnPull_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On pull"));
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "PullDefaultText"));
}

void UInventoryItem::OnPush_Implementation()
{
    IVerbInteractions::OnPush_Implementation();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On push"));
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "PushDefaultText"));
}

void UInventoryItem::OnUse_Implementation()
{
    IVerbInteractions::OnUse_Implementation();
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "UseDefaultText"));
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On use"));
}

void UInventoryItem::OnWalkTo_Implementation()
{
    IVerbInteractions::OnWalkTo_Implementation();
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "WalkToDefaultText"));
}

void UInventoryItem::OnItemUsed_Implementation()
{
    IVerbInteractions::OnItemUsed_Implementation();
    if (const auto Apc = AdventurePlayerController.Pin())
    {
        auto SourceKind = AdventurePlayerController->SourceItem->ItemKind;
        auto InteractableKind = AdventurePlayerController->SourceItem->InteractableItem;
        if (SourceKind == ItemKind)
        {
            // Item is used on itself
            AdventurePlayerController->InterruptCurrentAction();
            OnItemCombineFailure();
        }
        else if (SourceKind == InteractableItem || InteractableKind == ItemKind)
        {
            OnItemCombineSuccess();
        }
        else
        {
            // Item is not the one that can be used with this
            AdventurePlayerController->InterruptCurrentAction();
            OnItemCombineFailure();
        }
    }
}

void UInventoryItem::OnItemGiven_Implementation()
{
    IVerbInteractions::OnItemGiven_Implementation();
    if (const auto Apc = AdventurePlayerController.Pin())
    {
        // AdventurePlayerController->GiveAnItem(ItemKind);
    }
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemGivenDefaultText"));
}

void UInventoryItem::BarkAndEnd(FText BarkText)
{
    if (const auto Apc = AdventurePlayerController.Pin())
    {
        Apc->PlayerBark(BarkText);
        Apc->InterruptCurrentAction();
    }
}
