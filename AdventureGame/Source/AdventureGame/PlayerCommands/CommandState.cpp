// (c) 2025 Sarah Smith


#include "CommandState.h"

#include "ActiveTopState.h"
#include "FreeTopState.h"
#include "InstantActiveTopState.h"
#include "PendingTopState.h"
#include "PendingUseTopState.h"
#include "TargetingTopState.h"
#include "PendingGiveTopState.h"
#include "CommandStateMachine.h"
#include "AdventureGame/AdventureGame.h"

//////////////////////////////////
///
///  COMMAND STATE MACHINE
///

TSharedRef<ICommandState> MakeCommandStateClass(const ECommandCodes &Code)
{
    switch (Code) {

        /// Free
    case ECommandCodes::HoverScene:
        return MAKE_STATE(FHoverSceneState);
    case ECommandCodes::HoverInventory:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::HoverItem:
        return MAKE_STATE(FHoverItemState);
    case ECommandCodes::HoverHotSpot:
        return MAKE_STATE(FHoverHotSpotState);
    case ECommandCodes::HoverVerb:
        return MAKE_STATE(FHoverVerbState);

        /// Active 
    case ECommandCodes::WalkToHotSpot:
        return MAKE_STATE(FWalkToHotSpotState);
    case ECommandCodes::WalkToLocation:
        return MAKE_STATE(FWalkToLocationState);
    case ECommandCodes::LookAtItem:
        return MAKE_STATE(FLookAtItemState);

        /// Active and Pending
    case ECommandCodes::LookAt:
        return MAKE_STATE(FLookAtState);
    case ECommandCodes::Give:
        return MAKE_STATE(FGiveState);
    case ECommandCodes::Open:
        return MAKE_STATE(FOpenState);
    case ECommandCodes::Close:
        return MAKE_STATE(FCloseState);
    case ECommandCodes::PickUp:
        return MAKE_STATE(FPickUpState);
    case ECommandCodes::TalkTo:
        return MAKE_STATE(FTalkToState);
    case ECommandCodes::Use:
        return MAKE_STATE(FUseState);
    case ECommandCodes::Push:
        return MAKE_STATE(FPushState);
    case ECommandCodes::Pull:
        return MAKE_STATE(FPullState);

        /// Targeting
    case ECommandCodes::GiveTo:
        return MAKE_STATE(FGiveToState);
    case ECommandCodes::UseOn:
        return MAKE_STATE(FUseOnState);

        /// Top Level
    case ECommandCodes::Free:
        return MAKE_STATE(FFreeTopState);
    case ECommandCodes::Pending:
        return MAKE_STATE(FPendingTopState);
    case ECommandCodes::Targeting:
        return MAKE_STATE(FActiveTopState);
    case ECommandCodes::Active:
        return MAKE_STATE(FTargetingTopState);
    case ECommandCodes::InstantActive:
        return MAKE_STATE(FInstantActiveTopState);
    case ECommandCodes::PendingUse:
        return MAKE_STATE(FPendingUseTopState);
    case ECommandCodes::PendingGive:
        return MAKE_STATE(FPendingGiveTopState);

    default:
        // It's an error to try to create an `Initial` state class.
        UE_LOG(LogAdventureGame, Error, TEXT( "Unexpected state class creation request! %s"),
            *CommandCodesToString(Code));
    }
    return TSharedRef<ICommandState>();
}
