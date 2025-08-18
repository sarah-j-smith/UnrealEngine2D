#pragma once

#include "NativeGameplayTags.h"

namespace AdventureGameplayTags
{
    /// Schema is:
    ///     Scope - Location  -  What - Disposition
    ADVENTUREGAME_API  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Achievement_Gameplay_UnlockTowerExteriorDoor);
    
    ADVENTUREGAME_API  UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_TowerExterior_TowerDoor_Opened);
    ADVENTUREGAME_API  UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_TowerExterior_TowerDoor_Closed);
    
    ADVENTUREGAME_API  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_TowerExterior_Knife_PickedUp);
    ADVENTUREGAME_API  UE_DECLARE_GAMEPLAY_TAG_EXTERN(Item_TowerExterior_Gherkin_PickedUp);

    FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString);
};
