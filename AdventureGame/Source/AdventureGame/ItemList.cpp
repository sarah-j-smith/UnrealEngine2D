// (c) 2025 Sarah Smith


#include "ItemList.h"
#include "AdventureGame.h"
#include "Constants.h"
#include "ItemData.h"

FString UItemList::GetListDescription(const TArray<EItemList>& List)
{
	FString Accum = "";
	FString Concat = "";
	for (int i = 0; i < List.Num(); i++)
	{
		Accum += Concat + GetDescription(List[i]).ToString();
		Concat = ", ";
		if (i == List.Num() - 1) Concat = " and ";
	}
	return Accum;
}

FText UItemList::GetDescription(const EItemList &ItemKind)
{
	if (ItemKind == EItemList::None) return FText::GetEmpty();
	const FName ItemName = GetUniqueName(ItemKind);
	return FText::FromStringTable(ITEM_DESCRIPTIONS_KEY, ItemName.ToString());
}

FName UItemList::GetUniqueName(const EItemList &ItemKind)
{
	switch (ItemKind)
	{
	case EItemList::Pickle:
		return FName("Pickle");
	case EItemList::PickleKey:
		return FName("Pickle_Key");
	case EItemList::Knife:
		return FName("Knife");
	case EItemList::None:
	default:
		return FName("None");
	}
}

bool UItemList::Contains(EItemList Item) const
{
	for (const FInventoryList *Iterator = Inventory; Iterator; Iterator = Iterator->Next)
	{
		if (Iterator->Element->ItemKind == Item) return true;
	}
	return false;
}

bool UItemList::IsEmpty() const
{
	return Inventory == nullptr;
}

UInventoryItem *UItemList::AddItemToInventory(EItemList ItemToAdd, FText Description)
{
	if (ItemToAdd == EItemList::None)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("Refusing to add EItemList::None to inventory."));
		return nullptr;
	}
	FName ItemName = GetUniqueName(ItemToAdd);
	verifyf(InventoryDataTable, TEXT("ItemList could not load the InventoryDataTable"));
	FItemData* ItemData = InventoryDataTable->FindRow<FItemData>(ItemName, "AddItemToInventory");
	verifyf(ItemData, TEXT("ItemData could not be loaded for name \"%s\""), *ItemName.ToString());
	UInventoryItem* InventoryItem = NewObject<UInventoryItem>(this, ItemData->ItemClass, ItemName);
	InventoryItem->Description = Description;
	AddItemToInventory(InventoryItem);
	OnInventoryChanged.Broadcast(Identifier);
	if (UNLIKELY(GetWorld()->IsEditorWorld()))
	{
		DumpInventoryToLog();
	}
	return InventoryItem;
}

void UItemList::RemoveItemFromInventory(EItemList ItemToRemove)
{
	if (IsEmpty()) return;
	const TSet<EItemList> Items({ItemToRemove});
	RemoveItemListFromInventory(Items);
}


void UItemList::RemoveItemListFromInventory(const TSet<EItemList>& ItemsToRemove)
{
	if (IsEmpty()) return;
	FInventoryList *Item = Inventory;
	TSet<EItemList> RemovingItems(ItemsToRemove);
	for (FInventoryList *Iterator = Inventory; Iterator; Iterator = Iterator->Next)
	{
		if (RemovingItems.Contains(Iterator->Element->ItemKind))
		{
			RemovingItems.Remove(Iterator->Element->ItemKind);
			DeleteElementFromInventory(Iterator);
			if (RemovingItems.Num() == 0) break;			
		}
	}
	OnInventoryChanged.Broadcast(Identifier);
	if (RemovingItems.Num() > 0)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("Failed to remove some items from Inventory - %s"),
			*GetListDescription(ItemsToRemove.Array()));
	}
	if (UNLIKELY(GetWorld()->IsEditorWorld()))
	{
		DumpInventoryToLog();
	}
}

TArray<UInventoryItem *> UItemList::GetInventoryItemsArray() const
{
	TArray<UInventoryItem *> Result;
	for (const FInventoryList *Iterator = Inventory; Iterator; Iterator = Iterator->Next)
	{
		Result.Add(Iterator->Element);
	}
	return Result;
}


void UItemList::DumpInventoryToLog() const
{
	uint Index = 0;
	for (const FInventoryList *Iterator = Inventory; Iterator; Iterator = Iterator->Next)
	{
		FString Description = Iterator->Element->GetDescription().ToString();
		UE_LOG(LogAdventureGame, Verbose, TEXT("   %d - %s"), Index++, *Description);
	}
}

void UItemList::AddItemToInventory(UInventoryItem* InventoryItem)
{
	FInventoryList *NewItem = new FInventoryList(InventoryItem);
	if (Inventory)
	{
		// list exists case - add it as the new tail
		FInventoryList *Tail = nullptr;
		for (FInventoryList* Iterator = Inventory; Iterator; Tail = Iterator, Iterator = Iterator->Next) {}
		Tail->Next = NewItem;
	}
	else
	{
		// list is empty case, item becomes the new head
		Inventory = NewItem;
	}
	InventorySize++;
}

void UItemList::DeleteElementFromInventory(FInventoryList* Element)
{
	if (!Element) return;
	bool Deleted = false;
	FInventoryList *Tmp = Inventory;
	if (Element == Inventory)
	{
		Inventory = Inventory->Next;
		delete Tmp;
		Deleted = true;
	}
	else
	{
		for (FInventoryList *It = Inventory; It; Tmp = It, It = It->Next)
		{
			if (It == Element)
			{
				Tmp->Next = It->Next;
				Deleted = true;
				delete It;
				break;
			}
		}
	}
	verify(Deleted);
	InventorySize--;
}