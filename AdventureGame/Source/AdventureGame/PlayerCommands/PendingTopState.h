#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FPendingTopState final : public TFParentState<ECommandCodes::Pending>
{
public:
    virtual ~FPendingTopState() override = default;
    virtual bool CanTransition(const FStatePath &Destination) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

// The PLOCTOPP verbs are all the ones with a single target, a HotSpot or Item.

DECLARE_STATE_CLASS_DEFAULT(PickUp, PickUp)

DECLARE_STATE_CLASS_DEFAULT(LookAt, LookAt)

DECLARE_STATE_CLASS_DEFAULT(Open, Open)

DECLARE_STATE_CLASS_DEFAULT(Close, Close)

DECLARE_STATE_CLASS_DEFAULT(TalkTo, TalkTo)

DECLARE_STATE_CLASS_DEFAULT(Push, Push)

DECLARE_STATE_CLASS_DEFAULT(Pull, Pull)
