#include "ActiveTopState.h"

bool FActiveTopState::CanTransition(const FStatePath& Destination) const
{
    if (State->IsInitial())
    {
        return Destination.Parent == Code() && IsVerbCommandCode(Destination.Child);
    }
    // Once the verb has completed, or if its interrupted, there is nothing
    // to do but go back to the free state.
    return Destination.Parent == ECommandCodes::Free;
}

TOptional<EVerbType> FActiveTopState::GetVerb() const
{
    return State->GetCurrentVerb();
}

//////////////////////////////////
///
/// CHILD STATES
///
