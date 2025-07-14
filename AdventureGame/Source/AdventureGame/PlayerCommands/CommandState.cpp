// (c) 2025 Sarah Smith


#include "CommandState.h"

#include "AdventureGame/AdventureGame.h"

////////////// FCommandState - Base Template Class /////////////////

bool ICommandState::CanTransition(ECommandCodes Code) const
{
    UE_LOG(LogAdventureGame, Fatal, TEXT("CanTransition must be implemented by derived classes"));
    return false; // never reached.
}

void ICommandState::Transition(ECommandCodes Code)
{
    if (!CanTransition(Code))
    {
        UE_LOG(LogAdventureGame, Error, TEXT("Illegal transition: %s tried to transition to %s"),
            *Description(), *CommandCodesToString(Code))
        return;
    }
    Transition(Code);
}

bool ICommandState::IsInputLocked() const
{
    return false;
}

bool ICommandState::IsUsingItem() const
{
    return false;
}

bool ICommandState::IsInteractionHighlighted() const
{
    return true;
}

TOptional<EVerbType> ICommandState::GetVerb() const
{
    return TOptional<EVerbType>();
}

FString ICommandState::Description() const
{
    UE_LOG(LogAdventureGame, Error, TEXT("CanTransition must be implemented by derived classes"));
    return FString();
}

//////////////////////////////////
///
///  PARENT STATE
///

template <ECommandCodes E>
TFParentState<E>::TFParentState() = default;

template <ECommandCodes E>
TFParentState<E>::~TFParentState() = default;

//////////////////////////////////
///
///  FFreeTopState
///

TSet<ECommandCodes> FFreeTopState::ValidChildStates() const
{
    return TSet({
        ECommandCodes::HoverScene,
        ECommandCodes::HoverItem,
        ECommandCodes::HoverInventory,
        ECommandCodes::HoverHotSpot,
        ECommandCodes::HoverVerb
    });
}

bool FFreeTopState::CanTransition(ECommandCodes Code) const
{
    switch (Code)
    {
    case ECommandCodes::Pending:
    case ECommandCodes::Active:
        return true;
    default:
        return false;
    }
}

TOptional<EVerbType> FFreeTopState::GetVerb() const
{
    /// FIX ME
    return TOptional<EVerbType>();
}

//////////////////////////////////
///
///  TOP LEVEL STATE MACHINE
///

FTopLevelStateMachine MakeTopLevelStateMachine(const ECommandCodes StartingCommandCode)
{
    return {
        ECommandCodes::Free,
        MakeShareable(dynamic_cast<ICommandState*>(new FFreeTopState()))
    };
}

