#include "PendingUseTopState.h"

bool FPendingUseTopState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    case ECommandCodes::PendingUse:
        if (State->IsInitial()) return Destination.Child == ECommandCodes::Use;
        
    // Use key on door; Use knife on apple
    case ECommandCodes::Targeting:

    // Use lever
    case ECommandCodes::Active:

        return State->GetCurrentState()->CanTransition(Destination);

    default:
        return false;
    }
}

//////////////////////////////////
///
/// CHILD STATES
///

bool FUseState::CanTransition(const FStatePath& Destination) const
{
    if (Destination.Parent == ECommandCodes::Targeting)
    {
        return Destination.Child == ECommandCodes::UseOn;
    }
    else if (Destination.Parent == ECommandCodes::Active)
    {
        return Destination.Child == ECommandCodes::UseOn;
    }
    return false;
}

TOptional<EVerbType> FUseState::GetVerb() const
{
    return EVerbType::Use;
}
