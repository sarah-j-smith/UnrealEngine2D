// (c) 2025 Sarah Smith


#include "ItemList.h"

FString ItemGetDescriptiveString(const EItemList ItemList)
{
	switch (ItemList)
	{
	case EItemList::Pickle:
		return FString("Pickle");
	case EItemList::PickleKey:
		return FString("Pickle Key");
	case EItemList::Knife:
		return FString("Knife");
	case EItemList::None:
	default:
		return FString("None");
	}
}
