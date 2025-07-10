// (c) 2025 Sarah Smith


#include "InventoryItem.h"
#include "Constants.h"
#include "AdventureGame.h"
#include "AdventurePlayerController.h"
#include "AdventureGame/ItemList.h"
#include "Internationalization/StringTableRegistry.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////
///
/// STATIC IMPLEMENTATIONS
///

FText UInventoryItem::GetDescription(const UInventoryItem *Item)
{
    if (Item->Description.IsEmpty())
    {
        FText Text;
        if (UItemList *ItemList = Item->ItemList)
        {
            Text = ItemList->GetDescription(Item->ItemKind);
        }
        else
        {
            Text = FText::FromStringTable(ITEM_STRINGS_KEY, TEXT("DefaultItemDescriptionText"));
        }
        UE_LOG(LogAdventureGame, Warning, TEXT("Description for %s was not set"), *Text.ToString());
        return Text;
    }
    else
    {
        return Item->Description;
    }
}

FText UInventoryItem::GetDescription() const
{
    return UInventoryItem::GetDescription(this);
}

void UInventoryItem::OnItemCombineSuccess_Implementation()
{
    UE_LOG(LogAdventureGame, Log, TEXT("OnItemCombineSuccess Success - default."));
}

void UInventoryItem::OnItemCombineFailure_Implementation()
{
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemUsedDefaultText"));
}

void UInventoryItem::CombineWithInteractableItem(EItemKind ResultingItem, FText BarkText, FText Desc)
{
    if (const auto Apc = this->AdventurePlayerController)
    {
        if (Apc->CurrentItem)
        {
            Apc->CombineItems(
                Apc->CurrentItem,
                this, ResultingItem,
                BarkText.IsEmpty() ? LOCTABLE(ITEM_STRINGS_KEY, "CombineSuccessResultText") : BarkText,
                Desc.IsEmpty() ? LOCTABLE(ITEM_STRINGS_KEY, "DefaultCombineDescriptionText") : Desc
            );
            return;
        }
    }
    UE_LOG(LogAdventureGame, Warning,
        TEXT("Could not make %s - AdventurePlayerController->CurrentItem null."),
        *(Desc.IsEmpty() ? UItemList::GetDescription(ResultingItem).ToString() : Desc.ToString()));
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
        BarkAndEnd(GetDescription());
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
    if (AAdventurePlayerController *Apc = AdventurePlayerController)
    {
        Apc->UseAnItem(ItemKind);
    }
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
    if (AAdventurePlayerController *Apc = AdventurePlayerController)
    {
        if (AdventurePlayerController->ActiveItem == ItemKind)
        {
            // Item is used on itself
            AdventurePlayerController->InterruptCurrentAction();
            OnItemCombineFailure();
        }
        else if (AdventurePlayerController->ActiveItem == InteractableItem)
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
    if (AAdventurePlayerController *Apc = AdventurePlayerController)
    {
        AdventurePlayerController->GiveAnItem(ItemKind);
    }
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemGivenDefaultText"));
}

void UInventoryItem::BarkAndEnd(FText BarkText)
{
    if (AAdventurePlayerController *Apc = AdventurePlayerController)
    {
        Apc->PlayerBark(BarkText);
        Apc->InterruptCurrentAction();
    }
}
