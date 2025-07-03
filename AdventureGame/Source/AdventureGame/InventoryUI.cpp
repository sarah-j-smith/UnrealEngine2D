// (c) 2025 Sarah Smith


#include "InventoryUI.h"

#include "AdventureGame.h"
#include "AdventureGameInstance.h"
#include "AdventurePlayerController.h"
#include "Kismet/GameplayStatics.h"

void UInventoryUI::NativeOnInitialized()
{
	UpArrowButton->OnClicked.AddDynamic(this, &UInventoryUI::OnUpArrowButtonClicked);
	DownArrowButton->OnClicked.AddDynamic(this, &UInventoryUI::OnDownArrowButtonClicked);

	AddSlotsToArray();
}

void UInventoryUI::OnDownArrowButtonClicked()
{
	if (InventoryCount > 8 && CurrentRowIndex < MaxRowIndex)
	{
		CurrentRowIndex++;
		PopulateInventory();
	}
}

void UInventoryUI::OnUpArrowButtonClicked()
{
	if (InventoryCount > 8 && CurrentRowIndex > 0)
	{
		CurrentRowIndex--;
		PopulateInventory();
	}
}

void UInventoryUI::PopulateInventory(bool ScrollToLastAdded)
{
	UGameInstance *GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UAdventureGameInstance *AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
	InventoryCount = AdventureGameInstance->Inventory.Num();
	MaxRowIndex = InventoryCount > 8 ? ceilf((InventoryCount - 8) / 4.0f) : 0;

	UE_LOG(LogAdventureGame, Verbose, TEXT("PopulateInventory - Count: %d - MaxRowIndex: %d"), InventoryCount, MaxRowIndex);

	if (ScrollToLastAdded)
	{
		CurrentRowIndex = MaxRowIndex;
	}
	
	const int RowMin = CurrentRowIndex * 4;
	const int RowMax = RowMin + 7;
	int SlotIndex = 0;
	for (int i = RowMin; i <= RowMax; i++, SlotIndex++)
	{
		if (i < InventoryCount)
		{
			InventorySlots[SlotIndex]->AddItem(AdventureGameInstance->Inventory[i]);
		}
		else
		{
			InventorySlots[SlotIndex]->RemoveItem();
		}
	}
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
