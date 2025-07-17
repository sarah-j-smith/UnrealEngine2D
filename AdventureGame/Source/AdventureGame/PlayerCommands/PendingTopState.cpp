#include "PendingTopState.h"

bool FPendingTopState::CanTransition(const FStatePath& Destination) const
{
    switch (Destination.Parent)
    {
    case ECommandCodes::Active:
        // Pending state can transition to the active state for the same
        // verb/child. So if we are in Pending::Open (player clicked the
        // open verb) and asked CanTransition to Active::Open return true
        // which is what happens if the player clicks a target for "Open".
        return Destination.Child == State->GetCommandCode();
    default:
        return false;
    }
}

TOptional<EVerbType> FPendingTopState::GetVerb() const
{
    return State->GetCurrentVerb();
}

//////////////////////////////////
///
/// CHILD STATES
///

// No need to add code for these for CanTransition - all taken care of by
// parent CanTransition

TOptional<EVerbType> FPickUpState::GetVerb() const { return EVerbType::PickUp; }
TOptional<EVerbType> FLookAtState::GetVerb() const { return EVerbType::LookAt; }
TOptional<EVerbType> FOpenState::GetVerb() const { return EVerbType::Open; }
TOptional<EVerbType> FCloseState::GetVerb() const { return EVerbType::Close; }
TOptional<EVerbType> FTalkToState::GetVerb() const { return EVerbType::TalkTo; }
TOptional<EVerbType> FPushState::GetVerb() const { return EVerbType::Push; }
TOptional<EVerbType> FPullState::GetVerb() const { return EVerbType::PickUp; }
