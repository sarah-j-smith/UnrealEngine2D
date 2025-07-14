// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "CommandCodes.h"
#include "../VerbType.h"

/**
 * 
 */
template <ECommandCodes E>
class ADVENTUREGAME_API TFCommandState
{
public:
    virtual ~TFCommandState() = default;

    /// Return the Enum of the command code for this state.
    ECommandCodes Code() const { return E; }
};

class ADVENTUREGAME_API ICommandState
{
public:
    virtual ~ICommandState() = default;
    
    /// Given the current states, can this transition to the given state?
    /// This function effectively defines the state transition chart,
    /// and must be implemented by sub-classes.
    virtual bool CanTransition(ECommandCodes Code) const;

    /// Transition to the given state. If given the current states, its
    /// not possible, as defined by `CanTransition` log a warning and do nothing.
    /// This must be implemented by sub-classes.
    virtual void Transition(ECommandCodes Code);

    /// In this state can the player issue mouse clicks and other input 
    /// as normal, or is that **locked** so no input will be processed?
    virtual bool IsInputLocked() const;
    
    /// Does this state entail that the user is using an item, that is
    /// either giving it to a hotspot/item, or using it on a hotspot/item
    virtual bool IsUsingItem() const;

    /// Should the InteractionUI be highlighted? This indicates that a
    /// command instruction such as _Talk to Parrot_ is in progress.
    virtual bool IsInteractionHighlighted() const;

    /// If there is a current verb, return it, otherwise return an empty
    /// unset optional.
    virtual TOptional<EVerbType> GetVerb() const;

    virtual FString Description() const;
};

//////////////////////////////////
///
/// PARENT STATES
///

template <ECommandCodes E>
class TFParentState : public TFCommandState<E>, public ICommandState
{
public:
    TFParentState();

    explicit TFParentState(const TOptional<ECommandCodes>& StartingCommandCode = TOptional<ECommandCodes>())
    : CurrentChildCode(StartingCommandCode)
    {}

    virtual ~TFParentState() override;
    
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCommandChangedSignature, FName /* Parent */, FName /* Child */);
    
    /// If this has currently got a child state, return a valid optional containing
    /// that state, otherwise return an empty optional.
    virtual TOptional<ECommandCodes> CurrentChildState() const
    {
        return CurrentChildCode;
    }

    /// Set of states that are allowed to be child states of this.
    /// For leaf states this will be empty, which is the default.
    virtual TSet<ECommandCodes> ValidChildStates() const
    {
        return TSet<ECommandCodes>();
    }
    
private:
    TOptional<ECommandCodes> CurrentChildCode;
    TSharedRef<ICommandState> Current;
};

class ADVENTUREGAME_API FFreeTopState final : public TFParentState<ECommandCodes::Free>
{
public:
    FFreeTopState() : TFParentState<ECommandCodes::Free>(ECommandCodes::HoverScene) {}
    virtual ~FFreeTopState() override = default;
    virtual TSet<ECommandCodes> ValidChildStates() const override;
    virtual bool CanTransition(ECommandCodes Code) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};

class ADVENTUREGAME_API FPendingTopState final : public TFParentState<ECommandCodes::Pending>
{
public:
    FPendingTopState(ECommandCodes PendingStartCode) : TFParentState<ECommandCodes::Pending>(PendingStartCode) {};
    virtual ~FPendingTopState() override = default;
    virtual TSet<ECommandCodes> ValidChildStates() const override;
    virtual bool CanTransition(ECommandCodes Code) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};

class ADVENTUREGAME_API FActiveTopState final : public TFParentState<ECommandCodes::Active>
{
public:
    FActiveTopState(ECommandCodes ActiveStartCode) : TFParentState<ECommandCodes::Active>(ActiveStartCode) {}
    virtual ~FActiveTopState() override = default;
    virtual TSet<ECommandCodes> ValidChildStates() const override;
    virtual bool CanTransition(ECommandCodes Code) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};

class ADVENTUREGAME_API FTargetingTopState final : public TFParentState<ECommandCodes::Targeting>
{
public:
    FTargetingTopState(ECommandCodes TargetingStartCode) : TFParentState<ECommandCodes::Targeting>(TargetingStartCode) {}
    virtual ~FTargetingTopState() override = default;
    virtual TSet<ECommandCodes> ValidChildStates() const override;
    virtual bool CanTransition(ECommandCodes Code) const override;
    virtual TOptional<EVerbType> GetVerb() const override;
};

struct ADVENTUREGAME_API FTopLevelStateMachine
{
    ECommandCodes CurrentCommandCode;
    void Transition(ECommandCodes Code);
    TSharedRef<ICommandState> Current;
};

FTopLevelStateMachine MakeTopLevelStateMachine(const ECommandCodes StartingCommandCode);

//////////////////////////////////
///
/// PARENT STATES
///