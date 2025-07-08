#include "AdvGameUtils.h"

#include "AdventureGame.h"
#include "HotSpot.h"
#include "InventoryItem.h"

/**
 * Test to see if Current has changed more than DBL_EPSILON compared to Previous in either `X` or `Y`.
 * @param Current `FVector2D` Value to check to see if it differs
 * @param Previous `FVector2D` Value to check against as a baseline
 * @return true if `Current` is significantly different to `Previous`, false otherwise
 */
bool AdvGameUtils::HasChangedMuch(const FVector2D& Current, const FVector2D& Previous)
{
	return (fabs(Previous.X - Current.X) >= std::numeric_limits<double>::epsilon() || fabs(Previous.Y - Current.Y) >= std::numeric_limits<double>::epsilon());
}

int32 AdvGameUtils::GetUUID()
{
	uuid_t UUIDLatent;
	uuid_generate(UUIDLatent);
	return UUIDLatent[0] | UUIDLatent[1] << 8 | UUIDLatent[2] << 16 | UUIDLatent[3] << 24;
}

FString AdvGameUtils::GetGivingItemString(
		const UInventoryItem *CurrentItem,
		const UInventoryItem *TargetItem,
		AHotSpot *HotSpot
		)
{
	verifyf(CurrentItem, TEXT("GetGivingItemString expects CurrentItem to be non-null"));
	verifyf(TargetItem || HotSpot, TEXT("GetGivingItemString expects either TargetItem or HotSpot to be non-null"));
	FString InventoryText = CurrentItem->GetDescription().ToString();
	FString TargetItemString = HotSpot ? HotSpot->HotSpotDescription : TargetItem->GetDescription().ToString();
	return FString::Printf(TEXT("Give %s to %s"), *InventoryText, *TargetItemString);
}

FString AdvGameUtils::GetUsingItemString(const UInventoryItem* CurrentItem, const UInventoryItem* TargetItem, AHotSpot* HotSpot)
{
	verifyf(CurrentItem, TEXT("GetGivingItemString expects CurrentItem to be non-null"));
	verifyf(TargetItem || HotSpot, TEXT("GetGivingItemString expects either TargetItem or HotSpot to be non-null"));
	FString InventoryText = CurrentItem->GetDescription().ToString();
	FString TargetItemString = HotSpot ? HotSpot->HotSpotDescription : TargetItem->GetDescription().ToString();
	return FString::Printf(TEXT("Use %s on %s"), *TargetItem->GetName(), *TargetItemString);
}

FString AdvGameUtils::GetVerbWithItemString(const UInventoryItem* CurrentItem, const EVerbType Verb)
{
	FString ItemString = CurrentItem->GetDescription().ToString();
	return FString::Printf(TEXT("%s %s"),
		*VerbGetDescriptiveString(Verb), *ItemString);
}
