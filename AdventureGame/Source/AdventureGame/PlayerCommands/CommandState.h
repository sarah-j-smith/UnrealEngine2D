// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "CommandCodes.h"

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

/// Declare a state class with a default CanTransition method from the parent template
/// class that just returns false. The <code>GetVerb()</code> method must be defined.
#define DECLARE_STATE_CLASS_DEFAULT(ClassName, CommandEnum) \
class ADVENTUREGAME_API F ## ClassName ## State final : \
    public TFCommandState<ECommandCodes:: CommandEnum >, public ICommandState \
{ \
public: \
    virtual TOptional<EVerbType> GetVerb() const; \
    virtual ECommandCodes GetCode() const { return Code(); } \
};

/// Declare a state class with a custom CanTransition method which must be
/// provided, eg via an implementation in the CPP file
#define DECLARE_STATE_CLASS(ClassName, CommandEnum) \
    class ADVENTUREGAME_API F ## ClassName ## State final : \
        public TFCommandState<ECommandCodes:: CommandEnum >, public ICommandState \
{ \
    public: \
        virtual TOptional<EVerbType> GetVerb() const; \
        virtual ECommandCodes GetCode() const { return Code(); } \
        virtual bool CanTransition(const FStatePath &Destination) const override; \
};
