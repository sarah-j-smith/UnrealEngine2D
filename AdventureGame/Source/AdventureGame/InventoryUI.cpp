// (c) 2025 Sarah Smith


#include "InventoryUI.h"

void UInventoryUI::NativeOnInitialized()
{
	UpArrowButton->OnClicked.AddDynamic(this, &UInventoryUI::OnUpArrowButtonClicked);
	DownArrowButton->OnClicked.AddDynamic(this, &UInventoryUI::OnDownArrowButtonClicked);
}

void UInventoryUI::OnDownArrowButtonClicked()
{
	//
}

void UInventoryUI::OnUpArrowButtonClicked()
{
	//
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
