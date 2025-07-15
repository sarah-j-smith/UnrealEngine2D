// (c) 2025 Sarah Smith


#include "CommandState.h"

#include "ActiveTopState.h"
#include "FreeTopState.h"
#include "PendingTopState.h"
#include "TargetingTopState.h"

//////////////////////////////////
///
///  COMMAND STATE MACHINE
///

void FCommandStateMachine::Transition(const ECommandCodes &Code)
{
    CurrentCommandCode = Code;
    Current = MakeCommandStateClass(Code);
}

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
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::Give:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::Open:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::Close:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::PickUp:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::TalkTo:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::Use:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::Push:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::Pull:
        return MAKE_STATE(FHoverInventoryState);

        /*

        /// Targeting
    case ECommandCodes::GiveTo:
        return MAKE_STATE(FHoverInventoryState);
    case ECommandCodes::UseOn:
        return MAKE_STATE(FHoverInventoryState);

        /// Top Level
    case ECommandCodes::Free:
        return MAKE_STATE(FFreeTopState);
    case ECommandCodes::Pending:
        return MAKE_STATE(FPendingTopState);
    case ECommandCodes::Targeting:
        return MAKE_STATE(FActiveTopState);
    case ECommandCodes::Active:
        return MAKE_STATE(FTargetingTopState);

        */
    default:
        UE_LOG(LogAdventureGame, Error, TEXT( "Unexpected state class creation request! %s"),
            *CommandCodesToString(Code));
    }
    return TSharedRef<ICommandState>();
}

void FCommandStateMachine::Transition(const FStatePath &Path)
{
    Transition(Path.Parent);
    Current->Transition(Path.Child);
}

FCommandStateMachine MakeParentStateMachine(const ECommandCodes &StartingCode)
{
    return {
        StartingCode, 
        MAKE_STATE( FHoverSceneState ),
    };
}
