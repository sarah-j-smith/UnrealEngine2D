// (c) 2025 Sarah Smith


#include "CurrentCommand.h"

#include "AdventureGame/AdventureGame.h"

UCurrentCommand::UCurrentCommand(const ECommandCodes& StartingParentState, const ECommandCodes& StartingChildState)
    : State(MakeTopLevelStateMachine(StartingParentState))
    , StartingParentState(StartingParentState)
    , StartingChildState(StartingChildState)
{
    TopState = StartingParentState;
    ChildState = StartingChildState;
}

UCurrentCommand::UCurrentCommand()
    : UCurrentCommand(ECommandCodes::Free, ECommandCodes::HoverScene)
{
    UE_LOG(LogAdventureGame, Verbose, TEXT("UCurrentCommand constructed via default constructor"));
}

UCurrentCommand::UCurrentCommand(FObjectInitializer const&Initializer)
    : UCurrentCommand(ECommandCodes::Free, ECommandCodes::HoverScene)
{
    UE_LOG(LogAdventureGame, Verbose, TEXT("UCurrentCommand constructed via FObjectInitializer"));
}

FString UCurrentCommand::GetCommandString() const
{
    return FString::Printf(TEXT("%s::%s"), *CommandCodesToString(TopState), *CommandCodesToString(ChildState));
}

void UCurrentCommand::UseItem(UInventoryItem* Item)
{
}

void UCurrentCommand::GiveItem(UInventoryItem* Item)
{
}

void UCurrentCommand::WalkTo(AHotSpot* HotSpot)
{
}

void UCurrentCommand::WalkToLocation()
{
}

void UCurrentCommand::PerformVerb(EVerbType Verb)
{
}

void UCurrentCommand::Reset()
{
    TopState = StartingParentState;
    ChildState = StartingChildState;
    State = MakeTopLevelStateMachine(TopState);
}
