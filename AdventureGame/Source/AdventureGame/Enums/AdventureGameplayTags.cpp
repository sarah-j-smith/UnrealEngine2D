#include "AdventureGameplayTags.h"

#include "AdventureGame/AdventureGame.h"

namespace AdventureGameplayTags
{
    // If none of these tags is present for an item or hotspot then it's door state is "Unknown"
    // and cannot be locked, unlocked, opened, closed or walked through.
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Opened, "State.Opened", "The door or item is opened and accessible");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Closed, "State.Closed", "The door or item is closed and unlocked");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Locked, "State.Locked", "The door or item is closed and locked");

    // Status for various items in the objects that appear in the game. The default state is when the
    // tag is not present. For example if its not Hidden then its Visible.
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(HotSpot_Hidden, "HotSpot.Hidden", "The entire HotSpot actor is hidden");
    UE_DEFINE_GAMEPLAY_TAG_COMMENT(HotSpot_SpriteHidden, "HotSpot.SpriteHidden", "The PickUp sprite is hidden");


    void SetDoorState(EDoorState State, FGameplayTagContainer &Tags)
    {
        Tags.RemoveTag(AdventureGameplayTags::State_Closed);
        Tags.RemoveTag(AdventureGameplayTags::State_Opened);
        Tags.RemoveTag(AdventureGameplayTags::State_Locked);
        switch (State)
        {
        case EDoorState::Unknown:
            break;
        case EDoorState::Closed:
            Tags.AddTag(AdventureGameplayTags::State_Closed);
            break;
        case EDoorState::Opened:
            Tags.AddTag(AdventureGameplayTags::State_Opened);
            break;
        case EDoorState::Locked:
            Tags.AddTag(AdventureGameplayTags::State_Locked);
            break;
        }
    }

    EDoorState GetDoorState(const FGameplayTagContainer &Tags)
    {
        if (Tags.HasTag(AdventureGameplayTags::State_Closed)) return EDoorState::Closed;
        if (Tags.HasTag(AdventureGameplayTags::State_Opened)) return EDoorState::Opened;
        if (Tags.HasTag(AdventureGameplayTags::State_Locked)) return EDoorState::Locked;
        return EDoorState::Unknown;
    }
};