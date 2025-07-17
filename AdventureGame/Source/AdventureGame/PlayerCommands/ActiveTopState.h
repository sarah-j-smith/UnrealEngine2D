#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FActiveTopState final : public TFParentState<ECommandCodes::Active>
{
public:
    virtual bool CanTransition(const FStatePath &Destination) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
    virtual bool IsInteractionHighlighted() const override { return true;}
};

//////////////////////////////////
///
/// CHILD STATES
///

// The PLOCTPP verbs are children. These are declared in PendingTopState