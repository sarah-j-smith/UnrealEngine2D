#pragma once
#include "CommandCodes.h"
#include "CommandState.h"
#include "ParentState.h"

class ADVENTUREGAME_API FInstantActiveTopState final : public TFParentState<ECommandCodes::Active>
{
public:
    virtual bool CanTransition(const FStatePath &Destination) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(WalkToHotSpot, WalkToHotSpot)

DECLARE_STATE_CLASS(WalkToLocation, WalkToLocation)

DECLARE_STATE_CLASS(LookAtItem, LookAtItem)

