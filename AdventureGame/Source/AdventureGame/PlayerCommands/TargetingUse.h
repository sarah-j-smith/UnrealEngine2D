#pragma once

#include "ParentState.h"

/// Class for special case of a Use verb targeting a HotSpot.
class ADVENTUREGAME_API FTargetingUse final : public TFParentState<ECommandCodes::TargetingUse>
{
public:
    FTargetingUse() : TFParentState(ECommandCodes::HoverScene) {}
    virtual ~FTargetingUse() override = default;
    virtual bool CanTransition(const FStatePath &DestinationState) const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(UseOnHotSpot, UseOn)