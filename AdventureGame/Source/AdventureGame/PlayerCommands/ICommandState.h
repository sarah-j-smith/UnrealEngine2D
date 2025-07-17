#pragma once

#include "CommandCodes.h"


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
        return ECommandCodes::Initial;
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

    /// Get the current verb for this state. If its a parent, the class should
    /// delegate to the current child. Child states can derive the verb if there
    /// is one from their code; or if not, return an empty TOptional.
    /// The default implementation just returns an empty TOptional<EVerbType>
    virtual TOptional<EVerbType> GetVerb() const { return TOptional<EVerbType>(); }

    /// Create a string that describes the state, mostly for debug purposes.
    virtual FString Description() const { return ""; }
};
