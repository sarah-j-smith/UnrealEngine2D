// (c) 2025 Sarah Smith


#include "InventoryItem.h"

#include "AdventureGame.h"
#include "AdventurePlayerController.h"

UInventoryItem::UInventoryItem(FObjectInitializer const& ObjectInitializer)
	: Super(ObjectInitializer)
	, AdventurePlayerController(nullptr)
{
	//
}

void UInventoryItem::OnItemCombineSuccess_Implementation()
{
	UE_LOG(LogAdventureGame, Log, TEXT("OnItemCombineSuccess Success"));
}

void UInventoryItem::OnItemCombineFailure_Implementation()
{
	BarkAndEnd(ItemUsedDefaultText);
}

void UInventoryItem::OnClose_Implementation()
{
	IVerbInteractions::OnClose_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On close"));
	BarkAndEnd(CloseDefaultText);
}

void UInventoryItem::OnOpen_Implementation()
{
	IVerbInteractions::OnOpen_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On open"));
	BarkAndEnd(OpenDefaultText);
}

void UInventoryItem::OnGive_Implementation()
{
	IVerbInteractions::OnGive_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On give inventory item defaultasdfasdfasdf"));
	BarkAndEnd(GiveDefaultText);
}

void UInventoryItem::OnPickUp_Implementation()
{
	IVerbInteractions::OnPickUp_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On Pickup"));
	BarkAndEnd(PickUpDefaultText);
}

void UInventoryItem::OnTalkTo_Implementation()
{
	IVerbInteractions::OnTalkTo_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On talk"));
	BarkAndEnd(TalkToDefaultText);
}

void UInventoryItem::OnLookAt_Implementation()
{
	IVerbInteractions::OnLookAt_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On look at"));
	if (Description.IsEmpty())
	{
		BarkAndEnd(LookAtDefaultText);
	}
	else
	{
		BarkAndEnd(FText::FromString(Description));
	}
}

void UInventoryItem::OnPull_Implementation()
{
	IVerbInteractions::OnPull_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On pull"));
	BarkAndEnd(PullDefaultText);
}

void UInventoryItem::OnPush_Implementation()
{
	IVerbInteractions::OnPush_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On push"));
	BarkAndEnd(PushDefaultText);
}

void UInventoryItem::OnUse_Implementation()
{
	IVerbInteractions::OnUse_Implementation();
	AdventurePlayerController->ActiveItem = ItemKind;
	AdventurePlayerController->IsUsingItem = true;
	AdventurePlayerController->CurrentVerb = EVerbType::UseItem;
	AdventurePlayerController->TriggerUpdateInventoryText();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On use"));
}

void UInventoryItem::OnWalkTo_Implementation()
{
	IVerbInteractions::OnWalkTo_Implementation();
	BarkAndEnd(WalkToDefaultText);
}

void UInventoryItem::OnItemUsed_Implementation()
{
	IVerbInteractions::OnItemUsed_Implementation();

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

void UInventoryItem::OnItemGiven_Implementation()
{
	IVerbInteractions::OnItemGiven_Implementation();
	AdventurePlayerController->ActiveItem = ItemKind;
	AdventurePlayerController->IsGivingItem = true;
	AdventurePlayerController->CurrentVerb = EVerbType::GiveItem;
	AdventurePlayerController->TriggerUpdateInventoryText();
	BarkAndEnd(ItemGivenDefaultText);
}

void UInventoryItem::BarkAndEnd(const FText &BarkText)
{
	if (AdventurePlayerController)
	{
		AdventurePlayerController->PlayerBark(BarkText);
		AdventurePlayerController->InterruptCurrentAction();
	}
}
