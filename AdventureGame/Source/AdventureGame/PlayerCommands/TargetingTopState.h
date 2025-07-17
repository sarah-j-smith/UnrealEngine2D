#pragma once

#include "ParentState.h"

class ADVENTUREGAME_API FTargetingTopState final : public TFParentState<ECommandCodes::Targeting>
{
public:
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

constexpr FStatePath GTarget_UseOn { ECommandCodes::Targeting, ECommandCodes::UseOn };
constexpr FStatePath GTarget_GiveTo { ECommandCodes::Targeting, ECommandCodes::GiveTo };

constexpr FStatePath GActive_UseOn { ECommandCodes::Active, ECommandCodes::UseOn };
constexpr FStatePath GActive_GiveTo { ECommandCodes::Active, ECommandCodes::GiveTo };

