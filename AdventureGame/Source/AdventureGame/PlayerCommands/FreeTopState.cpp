#include "FreeTopState.h"

bool FFreeTopState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
        // Can transition to any other hover state just by moving the mouse
    case ECommandCodes::Free:
        return true;
    case ECommandCodes::Active:
    case ECommandCodes::Pending:
        return State.Current->CanTransition(Destination);
    default:
        return false;
    }
}

//////////////////////////////////
///
/// CHILD STATES
///

bool FHoverSceneState::CanTransition(const FStatePath& Destination) const
{
    // Player hovers the mouse over the scene somewhere and...
    // ... clicks on a location, to trigger active state > walking there.
    if (Destination.Parent == ECommandCodes::Active)
    {
        return Destination.Child == ECommandCodes::WalkToLocation;
    }
    return false;
}

bool FHoverInventoryState::CanTransition(const FStatePath& Destination) const
{
    // While over the inventory but not over an item, clicking just cancels
    // whatever item you had selected. That is handled in the Player controller.
    return false;
}

bool FHoverItemState::CanTransition(const FStatePath& Destination) const
{
    // Player hovers the mouse over an item in inventory and...
    // ... clicks on that item, to trigger active state > look at item
    if (Destination.Parent == ECommandCodes::Active)
    {
        return Destination.Child == ECommandCodes::LookAtItem;
    }
    return false;
}

bool FHoverVerbState::CanTransition(const FStatePath& Destination) const
{
    if (Destination.Parent == ECommandCodes::Pending)
    {
        return IsVerbCommandCode(Destination.Child);
    }
    return false;
}

bool FHoverHotSpotState::CanTransition(const FStatePath& Destination) const
{
    return true;
}
