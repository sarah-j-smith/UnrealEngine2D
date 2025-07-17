#include "PendingGiveTopState.h"

bool FPendingGiveTopState::CanTransition(const FStatePath& DestinationState) const
{
    if (DestinationState.Parent == ECommandCodes::Targeting)
    {
        return State->GetCurrentState()->CanTransition(DestinationState);
    }
    return false;
}


bool FGiveState::CanTransition(const FStatePath& Destination) const
{
    return Destination.Child == ECommandCodes::GiveTo;
}

TOptional<EVerbType> FGiveState::GetVerb() const
{
    return EVerbType::Give;
}
