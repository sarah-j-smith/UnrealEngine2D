#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FTargetingTopState final : public TFParentState<ECommandCodes::Targeting>
{
public:
    FTargetingTopState() : TFParentState(ECommandCodes::GiveTo) {}
    virtual ~FTargetingTopState() override = default;
    virtual bool CanTransition(const FStatePath &Destination) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};


//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(GiveTo, GiveTo)

DECLARE_STATE_CLASS(UseOn, UseOn)