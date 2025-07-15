#include "ActiveTopState.h"

bool FActiveTopState::CanTransition(const FStatePath& Destination) const
{
    // Once the verb has completed, or if its interrupted, there is nothing
    // to do but go back to the free state.
    return Destination.Parent == ECommandCodes::Free;
}

TOptional<EVerbType> FActiveTopState::GetVerb() const
{
    return GetVerbFromCommandCode(State.Current->GetCode());
}

//////////////////////////////////
///
/// CHILD STATES
///

bool FWalkToHotSpotState::CanTransition(const FStatePath& Destination) const
{
    return false;
}

bool FWalkToLocationState::CanTransition(const FStatePath& Destination) const
{
    return false;
}

bool FLookAtItemState::CanTransition(const FStatePath& Destination) const
{
    return false;
}
