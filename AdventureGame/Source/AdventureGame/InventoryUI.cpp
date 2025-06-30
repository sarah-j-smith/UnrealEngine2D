// (c) 2025 Sarah Smith


#include "InventoryUI.h"

#include "AdventureGameInstance.h"
#include "AdventurePlayerController.h"
#include "Kismet/GameplayStatics.h"

void UInventoryUI::NativeOnInitialized()
{
	UpArrowButton->OnClicked.AddDynamic(this, &UInventoryUI::OnUpArrowButtonClicked);
	DownArrowButton->OnClicked.AddDynamic(this, &UInventoryUI::OnDownArrowButtonClicked);

	AddSlotsToArray();
	CalculateMaxRowIndex();
	UpdateCurrentRowIndex();
}

void UInventoryUI::OnDownArrowButtonClicked()
{
	UpdateInventoryLength();
	if (iCurrentInventoryLength > 8)
	{
		bArrowButtonClicked = true;
	}
	if (CurrentRowIndex < MaxRowIndex)
	{
		CurrentRowIndex++;
	}
	PopulateInventory();
}

void UInventoryUI::OnUpArrowButtonClicked()
{
	UpdateInventoryLength();
	if (iCurrentInventoryLength > 8)
	{
		bArrowButtonClicked = true;
	}
	if (CurrentRowIndex > 0)
	{
		CurrentRowIndex--;
	}
	PopulateInventory();
}

void UInventoryUI::CalculateMaxRowIndex()
{
	UpdateInventoryLength();
	if (iCurrentInventoryLength > 8)
	{
		// Can scroll the rows via the arrow buttons
		MaxRowIndex = ceilf(static_cast<float>(iCurrentInventoryLength - 8) / 4.0f);
	}
	else
	{
		// All rows are visible with no scrolling
		MaxRowIndex = 0;
	}
}

void UInventoryUI::UpdateCurrentRowIndex()
{
	if (bArrowButtonClicked)
	{
		bArrowButtonClicked = false;	
	}
	CurrentRowIndex = MaxRowIndex;
}

void UInventoryUI::PopulateInventory()
{
	UGameInstance *GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UAdventureGameInstance *AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
	CalculateMaxRowIndex();
	const int RowMin = CurrentRowIndex * 4;
	const int RowMax = RowMin + 7;
	int SlotIndex = 0;
	for (int i = RowMin; i < RowMax; ++i, ++SlotIndex)
	{
		if (i < iCurrentInventoryLength)
		{
			InventorySlots[SlotIndex]->AddItem(AdventureGameInstance->Inventory[i]);
		}
		else
		{
			InventorySlots[SlotIndex]->RemoveItem();
		}
	}
}

void UInventoryUI::UpdateInventoryLength()
{
	UGameInstance *GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UAdventureGameInstance *AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
	iCurrentInventoryLength = AdventureGameInstance->Inventory.Num();
}

void UInventoryUI::AddSlotsToArray()
{
	InventorySlots.Reset(8);
	InventorySlots.Add(TopItem1);
	InventorySlots.Add(TopItem2);
	InventorySlots.Add(TopItem3);
	InventorySlots.Add(TopItem4);
	InventorySlots.Add(BottomItem1);
	InventorySlots.Add(BottomItem2);
	InventorySlots.Add(BottomItem3);
	InventorySlots.Add(BottomItem4);
}
