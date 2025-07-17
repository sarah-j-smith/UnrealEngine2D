#include "TargetingTopState.h"

bool FTargetingTopState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    case ECommandCodes::Active:
        return State->GetCurrentState()->CanTransition(Destination);
    default:
        return false;
    }
}

TOptional<EVerbType> FTargetingTopState::GetVerb() const
{
    return GetVerbFromCommandCode(State->GetCurrentState()->GetCode());
}

//////////////////////////////////
///
/// CHILD STATES
///

bool FGiveToState::CanTransition(const FStatePath& Destination) const
{
    // can only transition to Active :: GiveTo - parent checks Active
    return Destination.Child == ECommandCodes::GiveTo;    
}

TOptional<EVerbType> FGiveToState::GetVerb() const
{
    return EVerbType::GiveItem;
}

bool FUseOnState::CanTransition(const FStatePath& Destination) const
{
    // can only transition to Active :: UseOn - parent checks UseOn
    return Destination.Child == ECommandCodes::UseOn;
}

TOptional<EVerbType> FUseOnState::GetVerb() const
{
    return EVerbType::UseItem;
}
