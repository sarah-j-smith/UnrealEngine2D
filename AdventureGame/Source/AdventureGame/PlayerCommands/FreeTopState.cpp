#include "FreeTopState.h"

bool FFreeTopState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    // Can transition to other hover state by moving the mouse
    case ECommandCodes::Free:
    case ECommandCodes::InstantActive:
    case ECommandCodes::Pending:
    case ECommandCodes::PendingUse:
    case ECommandCodes::PendingGive:
        return State->GetCurrentState()->CanTransition(Destination);
    default:
        return false;
    }
}

// Here we default to the parent GetVerb which returns an empty optional.
// The free states do not have any very associated.

//////////////////////////////////
///
/// CHILD STATES
///

/////  Hover Scene State  /////////////////////////////

bool FHoverSceneState::CanTransition(const FStatePath& Destination) const
{
    // Player hovers the mouse over the scene somewhere and...
    // ... clicks on a location, to trigger active state > walking there.
    // Or, staying in the Hover parent state, hovers the inventory UI
    // or a hotspot in the scene.
    switch (Destination.Parent)
    {
    case ECommandCodes::Free:
        return Destination.Child == ECommandCodes::HoverInventory ||
            Destination.Child == ECommandCodes::HoverHotSpot;
    case ECommandCodes::InstantActive:
        return Destination.Child == ECommandCodes::WalkToLocation;
    default:
        return false;
    }
}

TOptional<EVerbType> FHoverSceneState::GetVerb() const
{
    return TOptional(EVerbType::WalkTo);
}

/////  Hover Inventory State  /////////////////////////////

bool FHoverInventoryState::CanTransition(const FStatePath& Destination) const
{
    // While over the inventory but not over an item, clicking just cancels
    // whatever item you had selected. That is handled in the Player controller.
    switch (Destination.Parent)
    {
    case ECommandCodes::Free:
        return Destination.Child == ECommandCodes::HoverVerb ||
            Destination.Child == ECommandCodes::HoverScene ||
            Destination.Child == ECommandCodes::HoverItem;
    default:
        return false;
    }
}

TOptional<EVerbType> FHoverInventoryState::GetVerb() const
{
    return TOptional(EVerbType::LookAt);
}

/////  Hover Item State  /////////////////////////////

bool FHoverItemState::CanTransition(const FStatePath& Destination) const
{
    // Player hovers the mouse over an item in inventory and...
    // ... clicks on that item, to trigger active state > look at item
    switch (Destination.Parent)
    {
    case ECommandCodes::Free:
        return Destination.Child == ECommandCodes::HoverInventory;
    case ECommandCodes::InstantActive:
        return Destination.Child == ECommandCodes::LookAtItem;
    default:
        return false;
    }
}

TOptional<EVerbType> FHoverItemState::GetVerb() const
{
    return TOptional(EVerbType::LookAt);
}

/////  Hover Verb State  /////////////////////////////

bool FHoverVerbState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    case ECommandCodes::Free:
        return Destination.Child == ECommandCodes::HoverInventory;
    case ECommandCodes::Pending:
        return IsVerbCommandCode(Destination.Child) &&
            Destination.Child != ECommandCodes::Use &&
            Destination.Child != ECommandCodes::Give;
    case ECommandCodes::PendingUse:
        return Destination.Child == ECommandCodes::Use;
    case ECommandCodes::PendingGive:
        return Destination.Child == ECommandCodes::Give;
    default:
        return false;
    }
}

TOptional<EVerbType> FHoverVerbState::GetVerb() const
{
    /// When hovering the verbs, the buttons light up. Rely on that, and don't populate
    /// the interaction text with a duplicate information.
    return TOptional<EVerbType>();
}

/////  Hover Scene State  /////////////////////////////

bool FHoverHotSpotState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    case ECommandCodes::Free:
        return Destination.Child == ECommandCodes::HoverScene;
    case ECommandCodes::InstantActive:
        return Destination.Child == ECommandCodes::WalkToHotSpot;
    default:
        return false;
    }
}

TOptional<EVerbType> FHoverHotSpotState::GetVerb() const
{
    return TOptional(EVerbType::WalkTo);
}
