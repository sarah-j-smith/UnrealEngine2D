#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FFreeTopState final : public TFParentState<ECommandCodes::Free>
{
public:
    FFreeTopState() : TFParentState(ECommandCodes::HoverScene) {}
    virtual ~FFreeTopState() override = default;
    virtual bool CanTransition(const FStatePath &DestinationState) const override;
};

//////////////////////////////////
///
/// CHILD STATES
///

DECLARE_STATE_CLASS(HoverScene, HoverScene)

DECLARE_STATE_CLASS(HoverInventory, HoverInventory)

DECLARE_STATE_CLASS(HoverVerb, HoverVerb)

DECLARE_STATE_CLASS(HoverHotSpot, HoverHotSpot)

DECLARE_STATE_CLASS(HoverItem, HoverItem)
