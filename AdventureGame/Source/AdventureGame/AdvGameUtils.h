#pragma once

#include "Enums/VerbType.h"

class AHotSpot;
class UInventoryItem;

struct AdvGameUtils
{
    static bool HasChangedMuch(const FVector2D& Current, const FVector2D& Previous);

    static int32 GetUUID();

    /**
     * For giving an item to a hotspot or item, return the text as an FText
     * translatable / i18n ready - for direct use in the UI. At least one
     * of CurrentItem or TargetItem must be non-null.
     * 
     * Uses the <code>ShortDescription</code> of the items and hotspots
     * so ensure these are correct: the default is "thing".
     * 
     * @param CurrentItem Item held by the player character
     * @param TargetItem Item  - if the target of the giving is an item
     * @param HotSpot HotSpot - if the target of the giving is a hotspot
     * @return FString text to display*/
    static FText GetGivingItemText(
        const UInventoryItem* CurrentItem,
        const UInventoryItem* TargetItem,
        const AHotSpot* HotSpot
    );

    /**
     * For using an item on a hotspot or item, return the text as an FText
     * translatable / i18n ready - for direct use in the UI. At least one
     * of CurrentItem or TargetItem must be non-null.
     * 
     * Uses the <code>ShortDescription</code> of the items and hotspots
     * so ensure these are correct: the default is "thing".
     * 
     * Example: "Use pen on old notebook", "Use pen on brick wall"
     * @param CurrentItem Item held by the player character, eg pen
     * @param TargetItem Item  - if the target of the using is an item
     * @param HotSpot HotSpot - if the target of the giving is a hotspot
     * @return FString text to display */
    static FText GetUsingItemText(
        const UInventoryItem* CurrentItem,
        const UInventoryItem* TargetItem,
        const AHotSpot* HotSpot
    );


    /**
     * For doing a verb to an item, return the text as an FText
     * translatable / i18n ready - for direct use in the UI. CurrentItem
     * must be non-null.
     * 
     * Uses the <code>ShortDescription</code> of the item
     * so ensure this is correct: the default is "thing".
     * 
    * Example: "Open tattered notebook"
    * @param CurrentItem Item held by the player character, eg notebook
    * @param Verb EVerbType  - what action is being performed
    * @return FString text to display
    */
    static FText GetVerbWithItemText(
        const UInventoryItem* CurrentItem,
        const EVerbType Verb
    );

    /**
     * For doing a verb to an hotspot, return the text as an FText
     * translatable / i18n ready - for direct use in the UI. Hotspot
     * must be non-null.
     * 
     * Uses the <code>ShortDescription</code> of the hotspot
     * so ensure this is correct: the default is "thing".
     * 
    * Example: "Open door"
    * @param HotSpot HotSpot targeted by the player character, eg door
    * @param Verb EVerbType  - what action is being performed
    * @return FText text to display
    */
    static FText GetVerbWithHotSpotText(
        const AHotSpot* HotSpot,
        const EVerbType Verb
    );
};
