// (c) 2025 Sarah Smith


#include "AdventureGameHUD.h"
#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "Kismet/GameplayStatics.h"
#include "AdventurePlayerController.h"
#include "AdvGameUtils.h"

void UAdventureGameHUD::NativeOnInitialized()
{
	PlayerCharacter = GetOwningPlayerPawn<AAdventureCharacter>();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *APC = Cast<AAdventurePlayerController>(PlayerController))
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
	auto CurrentVerb = AdventurePlayerController->CurrentVerb;
	if (auto CurrentHotspot = AdventurePlayerController->CurrentHotSpot)
	{
		FText hpStr = AdvGameUtils::GetVerbWithHotSpotText(CurrentHotspot, CurrentVerb);
		InteractionUI->SetText(hpStr);
		if (AdventurePlayerController->ShouldHighlightInteractionText())
		{
			InteractionUI->HighlightText();
		}
	}
	else
	{
		FText VerbStr = VerbGetDescriptiveString(CurrentVerb);
		InteractionUI->SetText(VerbStr);
	}
}

void UAdventureGameHUD::SetInventoryText()
{
	UItemSlot* ItemSlot = AdventurePlayerController->CurrentItemSlot;
	UInventoryItem* Item = nullptr; // Item a char is acting on from an item slot
	if (IsValid(ItemSlot) && ItemSlot->HasItem && IsValid(ItemSlot->InventoryItem))
	{
		Item = ItemSlot->InventoryItem;
	}
	else
	{
		InteractionUI->ResetText();
		return;
	}
	const UInventoryItem* CurrentItem = AdventurePlayerController->SourceItem; // Item a char has
	AHotSpot* HotSpot = AdventurePlayerController->CurrentHotSpot;
	const EVerbType Verb = AdventurePlayerController->CurrentVerb;
	FText InventoryText;
	bool ShouldHighlightText = false;
	if (Verb == EVerbType::UseItem)
	{
		InventoryText = AdvGameUtils::GetUsingItemText(CurrentItem, Item, HotSpot);
		ShouldHighlightText = true;
	}
	else if (Verb == EVerbType::GiveItem)
	{
		InventoryText = AdvGameUtils::GetGivingItemText(CurrentItem, Item, HotSpot);
		ShouldHighlightText = true;
	}
	else if (Item)
	{
		InventoryText = (Verb == EVerbType::WalkTo)
			                ? AdvGameUtils::GetVerbWithItemText(Item, EVerbType::LookAt)
			                : AdvGameUtils::GetVerbWithItemText(Item, Verb);
		ShouldHighlightText = AdventurePlayerController->ShouldHighlightInteractionText();
	}
	InteractionUI->SetText(InventoryText);
	if (ShouldHighlightText)
	{
		InteractionUI->HighlightText();
	}
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
