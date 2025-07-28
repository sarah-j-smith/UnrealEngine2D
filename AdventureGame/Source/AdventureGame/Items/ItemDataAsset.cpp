// (c) 2025 Sarah Smith


#include "ItemDataAsset.h"

#include "InventoryItem.h"
#include "ItemList.h"
#include "../Constants.h"
#include "../Player/AdventurePlayerController.h"
#include "AdventureGame/HotSpots/Door.h"


void UItemDataAsset::OnItemGiveSuccess_Implementation()
{
    if (AAdventurePlayerController *AdventurePlayerController = Apc.Get())
    {
        Apc->ItemRemoveFromInventory(SourceItem);
    }
    StartTimer();
}

void UItemDataAsset::OnItemUseSuccess_Implementation()
{
    if (AAdventurePlayerController *AdventurePlayerController = Apc.Get())
    {
        bool Success = true;
        UInventoryItem *NewItem = nullptr;
        switch (SourceItemAssetType)
        {
        case EItemAssetType::Consumable:
            Apc->ItemRemoveFromInventory(SourceItem);
            break;
        case EItemAssetType::Tool:
            NewItem = Apc->ItemAddToInventory(ToolResultItem);
            NewItem->SetAdventurePlayerController(AdventurePlayerController);
            break;
        case EItemAssetType::Key:
            if (AHotSpot *ThisHotSpot = AdventurePlayerController->CurrentHotSpot)
            {
                if (ADoor *Door = Cast<ADoor>(ThisHotSpot))
                {
                    Success = Door->UnlockDoor();
                    if (!Success && Door->DoorState != EDoorState::Locked)
                    {
                        AdventurePlayerController->PlayerBark(LOCTABLE(ITEM_STRINGS_KEY, "AlreadyUnlocked"));
                        
                    }
                }
            }
            else if (CanUnlockDoorOrItem(AdventurePlayerController->TargetItem->DoorState))
            {
                Success = true;
                AdventurePlayerController->TargetItem->DoorState = EDoorState::Closed;
            }
            break;
        default:
            break;
        }
        
        switch (TargetItemAssetType)
        {
        case EItemAssetType::Consumable:
            AdventurePlayerController->ItemRemoveFromInventory(TargetItem);
            break;
        default:
            break;
        }
        AdventurePlayerController->PlayerBark(Success ? UseSuccessBarkText : UseFailureBarkText);
        StartTimer();
    }
}

void UItemDataAsset::OnItemGiveFailure_Implementation()
{
    if (AAdventurePlayerController* AdventurePlayerController = Apc.Get())
    {
        AdventurePlayerController->PlayerBark(GiveFailureBarkText);
    }
}

void UItemDataAsset::OnItemUseFailure_Implementation()
{
    if (AAdventurePlayerController* AdventurePlayerController = Apc.Get())
    {
        AdventurePlayerController->PlayerBark(UseFailureBarkText);
    }
}

void UItemDataAsset::OnInteractionTimeout()
{
    if (AAdventurePlayerController* AdventurePlayerController = Apc.Get())
    {
        AdventurePlayerController->InterruptCurrentAction();
    }
}

void UItemDataAsset::SetAdventurePlayerController(AAdventurePlayerController* PlayerController)
{
    Apc = PlayerController;
}

void UItemDataAsset::StartTimer()
{
    if (TimerRunning) return;
    if (AAdventurePlayerController* AdventurePlayerController = Apc.Get())
    {
        AdventurePlayerController->InterruptCurrentAction();
        TimerRunning = true;
        AdventurePlayerController->GetWorldTimerManager().SetTimer(
            ActionHighlightTimerHandle, this,
            &UItemDataAsset::OnInteractionTimeout,
            1.0, false, InteractionTimeout);
    }
}

void UItemDataAsset::StopTimer()
{
    if (!TimerRunning) return;
    if (AAdventurePlayerController* AdventurePlayerController = Apc.Get())
    {
        TimerRunning = false;
        AdventurePlayerController->GetWorldTimerManager().ClearTimer(ActionHighlightTimerHandle);
    }
}
