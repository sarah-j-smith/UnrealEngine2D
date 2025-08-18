#include "AdventureGameplayTags.h"

#include "AdventureGame/AdventureGame.h"

namespace AdventureGameplayTags
{
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Achievement_Gameplay_UnlockTowerExteriorDoor, "Achievement.Gameplay.UnlockTowerExteriorDoor", "Player has done the gameplay to achieve unlock of the tower exterior door");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_TowerExterior_TowerDoor_Opened, "State.TowerExterior.TowerDoor.Opened", "The tower exterior door is opened");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_TowerExterior_TowerDoor_Closed, "State.TowerExterior.TowerDoor.Closed", "The tower exterior door is closed but unlocked");
    
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_TowerExterior_Knife_PickedUp, "Achievement.Gameplay.UnlockTowerExteriorDoor", "Player has done the gameplay to achieve unlock of the tower exterior door");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(Item_TowerExterior_Gherkin_PickedUp, "Achievement.Gameplay.UnlockTowerExteriorDoor", "Player has done the gameplay to achieve unlock of the tower exterior door");


    FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
    {
        const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
        FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

        if (!Tag.IsValid() && bMatchPartialString)
        {
            FGameplayTagContainer AllTags;
            Manager.RequestAllGameplayTags(AllTags, true);

            for (const FGameplayTag& TestTag : AllTags)
            {
                if (TestTag.ToString().Contains(TagString))
                {
                    UE_LOG(LogAdventureGame, Display,
                        TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."),
                        *TagString, *TestTag.ToString());
                    Tag = TestTag;
                    break;
                }
            }
        }

        return Tag;
    }
};