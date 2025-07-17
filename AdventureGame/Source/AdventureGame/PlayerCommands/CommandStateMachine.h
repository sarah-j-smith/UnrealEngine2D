#pragma once

#include "CommandCodes.h"
#include "ICommandState.h"
#include "AdventureGame/AdventureGame.h"

//////////////////////////////////
///
/// State Machine class instance and utils
///

/// Create an instance of the given <code>StateClass</code> which can be assigned to the
/// <code>TSharedRef<ICommandState></code> member variable in FCommandStateMachine.
#define MAKE_STATE(StateClass) MakeShareable(dynamic_cast<ICommandState*>(new StateClass()))

/// Lowest-level bare-bones FSM. The FSM is designed to handle a lot of possible states without
/// using a lot of memory.
///
/// It starts in the <code>CurrentCommandCode::Initial</code> state
/// where there is no instance of the <code>ICommandState</code> implementation
/// in memory/existence.
///
/// Instances are created only on transition into a state and
/// released when the TSharedPtr that holds them is re-assigned to the new state instance.
class ADVENTUREGAME_API FCommandStateMachine
{
public:
    /// Enum representing the value of the <code>Current</code> state.
    ECommandCodes GetCommandCode() const { return CurrentCommandCode; }

    /// Pointer to the current state object. This will be <code>nullptr</code>
    /// if the FSM is in an Initial state.
    /// @see <code>IsInitial()</code>
    ICommandState *GetCurrentState() const { return Current.Get(); }

    /// Verb type of the current state object. If the FSM is in a quiescent state.
    /// this will be an empty <code>TOptional<EVerbType></code>. This is a convenience
    /// method that calls <code>GetVerb()</code> on the current state, if it exists.
    /// @see <code>IsTerminal()</code>, <code>IsInitial()</code>
    TOptional<EVerbType> GetCurrentVerb() const
    {
        if (const ICommandState *Command = Current.Get())
        {
            return Command->GetVerb();
        }
        return TOptional<EVerbType>();
    }
    
    /// Transition the <code>Current</code> command to the given state, creating
    /// a new instance of the destination state class.
    ///
    /// No checking is done. Caller should check that the transition is valid
    /// in terms of the HSM rules via calls to <code>CanTransition</code>. No
    /// child states are changed.
    /// 
    /// @param Code command state to transition the <code>Current</code> command to.
    void Transition(const ECommandCodes &Code);

    /// Transition the <code>Current</code> command to the <code>Parent</code> in
    /// the state path, creating a new instance of the destination state class,
    /// then call <code>ICommandState::Transition()</code> on that new instance
    /// with the <code>Child</code> of the path.
    ///
    /// If the new <code>Parent</code> instance is a leaf state, and has no child
    /// then calling <code>ICommandState::Transition</code> on it will have
    /// <b>no effect</b>.
    /// 
    /// No checking is done. Caller should check that the transition is valid
    /// in terms of the HSM rules via calls to <code>CanTransition</code>.
    ///
    /// @param Path command state path to transition to.
    void Transition(const FStatePath &Path);

    /// Is the FSM in an initial state? If so <code>GetCurrentState()</code> will
    /// return nullptr.
    bool IsInitial() const { CheckCurrent(); return CurrentCommandCode == ECommandCodes::Initial; }

    void SetName(const FName &NewName) { DescriptiveName = NewName; }

private:
    FName DescriptiveName = "Command State Machine";
    ECommandCodes CurrentCommandCode = ECommandCodes::Initial;

    void CheckCurrent() const
    {
#if WITH_EDITOR
        if (CurrentCommandCode != ECommandCodes::Initial && !Current.IsValid())
        {
            UE_LOG(LogAdventureGame, Fatal,
                TEXT("%s - CurrentCommandCode == ECommandCodes::Initial -> Invalid Current Command!"),
                *DescriptiveName.ToString());
        }
#endif
    }
    
    /// <b>Do not modify directly</b>.
    /// The <code>Current</code> command state mananged by this FSM. TSharedPtr
    /// used here so no destructor required.
    TSharedPtr<ICommandState> Current;
};

FCommandStateMachine MakeStateMachine(const ECommandCodes &StartingCode);

TSharedRef<ICommandState> MakeCommandStateClass(const ECommandCodes &Code);
