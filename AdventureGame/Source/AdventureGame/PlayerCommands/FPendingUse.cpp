#include "FPendingUse.h"

bool FFPendingUse::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    // Can transition only to Use Targeting states just by moving the mouse
    case ECommandCodes::Targeting:
    case ECommandCodes::TargetingUse:
        return State.Current->CanTransition(Destination);
    default:
        return false;
    }
}

//////////////////////////////////
///
/// CHILD STATES
///

bool FUseState::CanTransition(const FStatePath& Destination) const { return false; }