// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "CommandCodes.h"
#include "../VerbType.h"

/**
 * Try to model the complete operation of the UI and valid command states for the 9-verb adventure
 * game. https://docs.google.com/spreadsheets/d/1y92Zvv8v7AZhG5S9i43j6uzMPU-zOH4BFU9vJfna2fw/edit?gid=0#gid=0
 */

//////////////////////////////////
///
/// Command State Machine base definitions
///

class AHotSpot;
class UInventoryItem;
class ICommandState;

struct FStatePath
{
    /// Parent state of hierarchical state graph
    ECommandCodes Parent;
    /// Child state of hierarchical state graph
    ECommandCodes Child;
};

constexpr FStatePath GWalk_To_Location { ECommandCodes::Active, ECommandCodes::WalkToLocation };
constexpr FStatePath GWalk_To_Hotspot { ECommandCodes::Active, ECommandCodes::WalkToHotSpot };
constexpr FStatePath GLook_At_Item { ECommandCodes::Active, ECommandCodes::LookAtItem };

/**
 * Base class of all command states, parent and child.
 */
template <ECommandCodes E>
class ADVENTUREGAME_API TFCommandState
{
public:
    TFCommandState();
    virtual ~TFCommandState();

    /// Return the Enum of the command code for this state.
    ECommandCodes Code() const { return E; }
};

template <ECommandCodes E>
TFCommandState<E>::TFCommandState() = default;

template <ECommandCodes E>
TFCommandState<E>::~TFCommandState() = default;

//////////////////////////////////
///
/// Macro to Declare State Machine leaf states
///

#define DECLARE_STATE_CLASS(ClassName, CommandEnum) \
class ADVENTUREGAME_API F ## ClassName ## State final : \
    public TFCommandState<ECommandCodes:: CommandEnum >, public ICommandState \
{ \
public: \
    virtual ECommandCodes GetCode() const { return Code(); } \
    virtual bool CanTransition(const FStatePath &Destination) const override; \
    void SetParent(TSharedRef<ICommandState> Parent) { this->Parent = Parent; } \
private: \
    TWeakPtr<ICommandState> Parent; \
};

//////////////////////////////////
///
/// State Machine class instance and utils
///

/// Create an instance of the given <code>StateClass</code> which can be assigned to the
/// <code>TSharedRef<ICommandState></code> member variable in FCommandStateMachine.
#define MAKE_STATE(StateClass) MakeShareable(dynamic_cast<ICommandState*>(new StateClass()))

struct ADVENTUREGAME_API FCommandStateMachine
{
    ECommandCodes CurrentCommandCode;
    
    /// Transition to the given state, creating a new instance of the destination.
    /// No checking is done. Caller should check that the transition is valid
    /// in terms of the HSM rules via calls to <code>CanTransition</code>.
    void Transition(const ECommandCodes &Code);

    void Transition(const FStatePath &Path);
    
    TSharedRef<ICommandState> Current;
};

FCommandStateMachine MakeTopLevelStateMachine(const FStatePath &StartingPath);

FCommandStateMachine MakeParentStateMachine(const ECommandCodes &StartingCode);

TSharedRef<ICommandState> MakeCommandStateClass(const ECommandCodes &Code);

/**
 * Interface definition for a State of the player command system. Includes
 * both the parent and child states, as there is a two level HSM or State Chart.
 *
 * [State Transition table](https://docs.google.com/spreadsheets/d/1y92Zvv8v7AZhG5S9i43j6uzMPU-zOH4BFU9vJfna2fw/edit?usp=sharing)
 */
class ADVENTUREGAME_API ICommandState
{
public:
    virtual ~ICommandState() = default;

    virtual ECommandCodes GetCode() const
    {
        return ECommandCodes::WalkToLocation;
    };
    
    /// Given the current states, can this transition to the given <b>sibling</b> state?
    /// This function effectively defines the state transition chart,
    /// and should usually be implemented by sub-classes. Default version simply checks if
    /// <code>CodeToTransitionTo</code> is a member of the set returned by
    /// <code>ValidSiblingStates</code>
    /// @param DestinationState StatePath for target state to transition to
    /// @return true if a parent state machine can transition from <b>this</b> state to the target state.
    /// @see ValidSiblingStates
    virtual bool CanTransition(const FStatePath &DestinationState) const { return false; }

    virtual void Transition(ECommandCodes Code) {};
    
    /// In this state can the player issue mouse clicks and other input 
    /// as normal, or is that **locked** so no input will be processed?
    virtual bool IsInputLocked() const { return false; }
    
    /// Does this state entail that the user is using an item, that is
    /// either giving it to a hotspot/item, or using it on a hotspot/item
    virtual bool IsUsingItem() const { return false; }

    /// Should the InteractionUI be highlighted? This indicates that a
    /// command instruction such as _Talk to Parrot_ is in progress.
    virtual bool IsInteractionHighlighted() const { return false; }

    /// If there is a current verb, return it, otherwise return an empty
    /// unset optional.
    virtual TOptional<EVerbType> GetVerb() const
    {
        return TOptional<EVerbType>();
    }

    /// Capture a pointer to the parent state. The default implementation
    /// does nothing, and parent classes should use this. Child classes
    /// can use this to capture a weak pointer locally.
    virtual void SetParent(TSharedRef<ICommandState> Parent) {}

    /// Create a string that describes the state, mostly for debug purposes.
    virtual FString Description() const { return ""; };
};
