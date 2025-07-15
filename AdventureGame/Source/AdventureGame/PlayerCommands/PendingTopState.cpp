#include "PendingTopState.h"

bool FPendingTopState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    case ECommandCodes::Active:
        // By selecting a current item/hotspot for the verb do a transition from pending to active
        return true;
    case ECommandCodes::Targeting:
        // A few pending states can transition to targeting once the source item/hotspot is chosen
        return State.Current->CanTransition(Destination);
    default:
        return false;
    }
}

TOptional<EVerbType> FPendingTopState::GetVerb() const
{
    return GetVerbFromCommandCode(State.Current->GetCode());
}


//////////////////////////////////
///
/// CHILD STATES
///

bool FLookAtState::CanTransition(const FStatePath& Destination) const { return false; }

bool FGiveState::CanTransition(const FStatePath& Destination) const { return false; }

bool FOpenState::CanTransition(const FStatePath& Destination) const { return false; }

bool FCloseState::CanTransition(const FStatePath& Destination) const { return false; }

bool FPickUpState::CanTransition(const FStatePath& Destination) const { return false; }

bool FTalkToState::CanTransition(const FStatePath& Destination) const { return false; }

bool FUseState::CanTransition(const FStatePath& Destination) const { return false; }

bool FPushState::CanTransition(const FStatePath& Destination) const { return false; }

bool FPullState::CanTransition(const FStatePath& Destination) const { return false; }

