#include "CommandStateMachine.h"

void FCommandStateMachine::Transition(const ECommandCodes &Code)
{
    // Don't transition back into the state we are already in. 
    if (!IsInitial() && Code == Current->GetCode())
    {
#if WITH_EDITOR
        UE_LOG(LogAdventureGame, Warning, TEXT("%s - Attempt to transition to the current state %s"),
            *DescriptiveName.ToString(),
            *CommandCodesToString(Code));
#endif
        return;
    }
    CurrentCommandCode = Code;
    Current = MakeCommandStateClass(Code);
}

void FCommandStateMachine::Transition(const FStatePath &Path)
{
    Transition(Path.Parent);
    Current->Transition(Path.Child);
}
