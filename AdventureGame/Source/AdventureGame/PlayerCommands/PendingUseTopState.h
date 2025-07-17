#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FPendingUseTopState final : public TFParentState<ECommandCodes::PendingUse>
{
public:
    FPendingUseTopState() : TFParentState() {}
    virtual ~FPendingUseTopState() override = default;
    virtual bool CanTransition(const FStatePath &DestinationState) const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(Use, Use)


