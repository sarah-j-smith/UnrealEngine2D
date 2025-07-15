#include "TargetingTopState.h"

bool FTargetingTopState::CanTransition(const FStatePath& Destination) const
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

TOptional<EVerbType> FTargetingTopState::GetVerb() const
{
    return GetVerbFromCommandCode(State.Current->GetCode());
}
