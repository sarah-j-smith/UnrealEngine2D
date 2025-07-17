#pragma once

#include "ParentState.h"

/// Class for special case of a Use verb targeting a HotSpot.
class ADVENTUREGAME_API FPendingGiveTopState final : public TFParentState<ECommandCodes::PendingGive>
{
public:
    virtual ~FPendingGiveTopState() override = default;
    virtual bool CanTransition(const FStatePath &DestinationState) const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(Give, Give)