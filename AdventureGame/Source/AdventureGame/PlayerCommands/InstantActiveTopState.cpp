#include "InstantActiveTopState.h"

bool FInstantActiveTopState::CanTransition(const FStatePath& Destination) const
{
    return false;
}

TOptional<EVerbType> FInstantActiveTopState::GetVerb() const
{
    return State->GetCurrentVerb();
}

//////////////////////////////////
///
/// CHILD STATES
///

/////  WalkTo HotSpot State  /////////////////////////////

bool FWalkToHotSpotState::CanTransition(const FStatePath& Destination) const
{
    return false;
}

TOptional<EVerbType> FWalkToHotSpotState::GetVerb() const
{
    return EVerbType::WalkTo;
}

/////  WalkTo Location State  /////////////////////////////

bool FWalkToLocationState::CanTransition(const FStatePath& Destination) const
{
    return false;
}

TOptional<EVerbType> FWalkToLocationState::GetVerb() const
{
    return EVerbType::WalkTo;
}

/////  LookAt Item State  /////////////////////////////

bool FLookAtItemState::CanTransition(const FStatePath& Destination) const
{
    return false;
}

TOptional<EVerbType> FLookAtItemState::GetVerb() const
{
    return EVerbType::LookAt;
}
