// (c) 2025 Sarah Smith


#include "ItemSlot.h"

#include "AdventurePlayerController.h"
#include "InventoryItem.h"
#include "Kismet/GameplayStatics.h"

void UItemSlot::NativeOnInitialized()
{
	ItemButton->OnClicked.AddDynamic(this, & UItemSlot::HandleOnClicked);
	ItemButton->OnHovered.AddDynamic(this, & UItemSlot::HandleOnHover);
	ItemButton->OnUnhovered.AddDynamic(this, & UItemSlot::HandleOnUnhover);

	if (!HasItem)
	{
		ItemSlot->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlot::AddItem(UInventoryItem* InventoryItem)
{
	if (InventoryItem != nullptr)
	{
		HasItem = true;
		this->InventoryItem = InventoryItem;
		SetButtonImageFromInventoryItem(InventoryItem);
		ItemSlot->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemSlot::RemoveItem()
{
	if (HasItem)
	{
		HasItem = false;
		this->InventoryItem = nullptr;
		ItemButton->SetStyle(SavedStyle);
		ItemSlot->SetVisibility(ESlateVisibility::Visible);
	}
}

void UItemSlot::HandleOnClicked()
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController))
	{
		if (HasItem)
		{
			AdventurePlayerController->HandleInventoryItemClicked(this);
		}
		else
		{
			// Clicked on an empty slot so cancel whatever it was doing.
			AdventurePlayerController->InterruptCurrentAction();
		}
	}
}

void UItemSlot::HandleOnHover()
{
	if (HasItem)
	{
		APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController))
		{
			AdventurePlayerController->MouseEnterInventoryItem(this);
		}		
	}
}

void UItemSlot::HandleOnUnhover()
{
}

void UItemSlot::SetButtonImageFromInventoryItem(const UInventoryItem* InventoryItem)
{
	FButtonStyle Style = ItemButton->GetStyle();
	SavedStyle = Style;
	Style.Normal.SetResourceObject(InventoryItem->Thumbnail->GetSourceTexture());
	ItemButton->SetStyle(Style);
}

