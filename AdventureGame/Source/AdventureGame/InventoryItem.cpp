// (c) 2025 Sarah Smith


#include "InventoryItem.h"
#include "Constants.h"
#include "AdventureGame.h"
#include "AdventurePlayerController.h"
#include "Internationalization/StringTableRegistry.h"

FText UInventoryItem::GetDescription(const UInventoryItem *Item)
{
    if (Item->Description.IsEmpty())
    {
        FText Text;
        if (UItemList *ItemList = Item->ItemList.Get())
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

UInventoryItem::UInventoryItem(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
      , AdventurePlayerController(nullptr)
{
    //
}

void UInventoryItem::OnItemCombineSuccess_Implementation()
{
    UE_LOG(LogAdventureGame, Log, TEXT("OnItemCombineSuccess Success - default."));
}

void UInventoryItem::OnItemCombineFailure_Implementation()
{
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemUsedDefaultText"));
}

void UInventoryItem::CombineWithInteractableItem(EItemList ResultingItem, FText BarkText, FText Desc)
{
    if (const TSharedPtr<AAdventurePlayerController> AdventurePlayerController = this->AdventurePlayerController.Pin())
    {
        if (AdventurePlayerController->CurrentItem)
        {
            AdventurePlayerController->CombineItems(
                AdventurePlayerController->CurrentItem,
                this, ResultingItem,
                BarkText.IsEmpty() ? LOCTABLE(ITEM_STRINGS_KEY, "CombineSuccessResultText") : BarkText,
                Desc.IsEmpty() ? LOCTABLE(ITEM_STRINGS_KEY, "DefaultCombineDescriptionText") : Desc
            );
            return;
        }
        const FString DebugString = Desc.IsEmpty() ? UItemList::GetDescription(ResultingItem).ToString() : Desc.ToString();
        UE_LOG(LogAdventureGame, Warning, TEXT("Could not make %s - AdventurePlayerController->CurrentItem null."),
            *(DebugString));
    }
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
    if (const TSharedPtr<AAdventurePlayerController> AdventurePlayerController = this->AdventurePlayerController.Pin())
    {
        AdventurePlayerController->UseAnItem(ItemKind);
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
    if (const TSharedPtr<AAdventurePlayerController> AdventurePlayerController = this->AdventurePlayerController.Pin())
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
    if (const TSharedPtr<AAdventurePlayerController> AdventurePlayerController = this->AdventurePlayerController.Pin())
    {
        AdventurePlayerController->GiveAnItem(ItemKind);
    }
    BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "ItemGivenDefaultText"));
}

AAdventurePlayerController *UInventoryItem::GetAdventureController() const
{
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    return Cast<AAdventurePlayerController>(PlayerController);
}

void UInventoryItem::BarkAndEnd(const FText& BarkText)
{
    if (const TSharedPtr<AAdventurePlayerController> AdventurePlayerController = this->AdventurePlayerController.Pin())
    {
        AdventurePlayerController->PlayerBark(BarkText);
        AdventurePlayerController->InterruptCurrentAction();
    }
}
