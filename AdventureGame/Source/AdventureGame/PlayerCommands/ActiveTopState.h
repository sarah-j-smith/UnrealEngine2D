#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FActiveTopState final : public TFParentState<ECommandCodes::Active>
{
public:
    FActiveTopState() : TFParentState(ECommandCodes::WalkToLocation) {}
    virtual ~FActiveTopState() override = default;
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

