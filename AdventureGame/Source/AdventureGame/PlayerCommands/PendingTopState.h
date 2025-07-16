#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FPendingTopState final : public TFParentState<ECommandCodes::Pending>
{
public:
    FPendingTopState() : TFParentState(ECommandCodes::LookAt) {};
    virtual ~FPendingTopState() override = default;
    virtual bool CanTransition(const FStatePath &Destination) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(LookAt, LookAt)

DECLARE_STATE_CLASS(Give, Give)

DECLARE_STATE_CLASS(Open, Open)

DECLARE_STATE_CLASS(Close, Close)

DECLARE_STATE_CLASS(PickUp, PickUp)

DECLARE_STATE_CLASS(TalkTo, TalkTo)

DECLARE_STATE_CLASS(Push, Push)

DECLARE_STATE_CLASS(Pull, Pull)
