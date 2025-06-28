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

void UInventoryItem::OnItemCombineSuccess()
{
	UE_LOG(LogAdventureGame, Log, TEXT("OnItemCombineSuccess Success"));
}

void UInventoryItem::OnItemCombineFailure()
{
	Bark(ItemUsedDefaultText);
}

void UInventoryItem::OnClose_Implementation()
{
	IVerbInteractions::OnClose_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On close"));
	Bark(CloseDefaultText);
}

void UInventoryItem::OnOpen_Implementation()
{
	IVerbInteractions::OnOpen_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On open"));
	Bark(OpenDefaultText);
}

void UInventoryItem::OnGive_Implementation()
{
	IVerbInteractions::OnGive_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On give"));
	Bark(GiveDefaultText);
}

void UInventoryItem::OnPickUp_Implementation()
{
	IVerbInteractions::OnPickUp_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On Pickup"));
	Bark(PickUpDefaultText);
}

void UInventoryItem::OnTalkTo_Implementation()
{
	IVerbInteractions::OnTalkTo_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On talk to"));
	Bark(TalkToDefaultText);
}

void UInventoryItem::OnLookAt_Implementation()
{
	IVerbInteractions::OnLookAt_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On look at"));
	Bark(LookAtDefaultText);
}

void UInventoryItem::OnPull_Implementation()
{
	IVerbInteractions::OnPull_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On pull"));
	Bark(PullDefaultText);
}

void UInventoryItem::OnPush_Implementation()
{
	IVerbInteractions::OnPush_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On push"));
	Bark(PushDefaultText);
}

void UInventoryItem::OnUse_Implementation()
{
	IVerbInteractions::OnUse_Implementation();
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("On use"));
	Bark(UseDefaultText);
}

void UInventoryItem::OnWalkTo_Implementation()
{
	// WalkToDefaultText
}

void UInventoryItem::OnItemUsed_Implementation(EItemList ItemUsed)
{
	if (ItemUsed == InteractableItem && InteractableItem != EItemList::None)
	{
		IVerbInteractions::OnItemUsed_Implementation(ItemUsed);
		OnItemCombineSuccess();
	}
	else
	{
		OnItemCombineFailure();
	}
}

void UInventoryItem::Bark(const FText &BarkText)
{
	if (AdventurePlayerController)
	{
		AdventurePlayerController->PlayerBark(BarkText);
	}
}
