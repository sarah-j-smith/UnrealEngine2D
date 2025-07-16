#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FFPendingUse final : public TFParentState<ECommandCodes::PendingUse>
{
public:
    FFPendingUse() : TFParentState(ECommandCodes::HoverScene) {}
    virtual ~FFPendingUse() override = default;
    virtual bool CanTransition(const FStatePath &DestinationState) const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(Use, Use)

