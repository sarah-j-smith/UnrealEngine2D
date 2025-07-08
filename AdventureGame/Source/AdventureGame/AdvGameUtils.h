#pragma once
#include "VerbType.h"

class AHotSpot;
class UInventoryItem;

struct AdvGameUtils
{
    static bool HasChangedMuch(const FVector2D& Current, const FVector2D& Previous);

    static int32 GetUUID();

    /**
     * Return the text as an FString for direct use in the UI, for giving
     * an item to another hotspot or item. At least one of CurrentItem or
     * TargetItem must be non-null.
     * @param CurrentItem Item held by the player character
     * @param TargetItem Item  - if the target of the giving is an item
     * @param HotSpot HotSpot - if the target of the giving is a hotspot
     * @return FString text to display*/
    static FString GetGivingItemString(
        const UInventoryItem* CurrentItem,
        const UInventoryItem* TargetItem,
        AHotSpot* HotSpot
    );

    /**
     * Return the text as an FString for direct use in the UI, for using
     * an item on another hotspot or item.  At least one of CurrentItem or
     * TargetItem must be non-null.
     * Example: "Use pen on notebook", "Use pen on wall"
     * @param CurrentItem Item held by the player character, eg pen
     * @param TargetItem Item  - if the target of the using is an item
     * @param HotSpot HotSpot - if the target of the giving is a hotspot
     * @return FString text to display */
    static FString GetUsingItemString(
        const UInventoryItem* CurrentItem,
        const UInventoryItem* TargetItem,
        AHotSpot* HotSpot
    );


    /**
    * Return the text as an FString for direct use in the UI, for doing
    * a verb to an item.
    * Example: "Open notebook"
    * @param CurrentItem Item held by the player character, eg notebook
    * @param Verb EVerbType  - what action is being performed
    * @return FString text to display
    */
    static FString GetVerbWithItemString(
        const UInventoryItem* CurrentItem,
        const EVerbType Verb
    );
};
