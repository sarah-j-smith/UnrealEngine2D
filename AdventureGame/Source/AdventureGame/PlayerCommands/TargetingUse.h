#pragma once

#include "PlayerCommands/ParentState.h"

class ADVENTUREGAME_API TargetingUse final : public TFParentState<ECommandCodes::Free>
    {
    public:
        FFreeTopState() : TFParentState(ECommandCodes::HoverScene) {}
        virtual ~FFreeTopState() override = default;
        virtual bool CanTransition(const FStatePath &DestinationState) const override;
    };
