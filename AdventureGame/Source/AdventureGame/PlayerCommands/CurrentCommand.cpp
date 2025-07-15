// (c) 2025 Sarah Smith


#include "CurrentCommand.h"

#include "AdventureGame/AdventureGame.h"

UCurrentCommand::UCurrentCommand(const ECommandCodes& StartingParentState, const ECommandCodes& StartingChildState)
    : State(MakeParentStateMachine(StartingParentState))
    , StartingParentState(StartingParentState)
    , StartingChildState(StartingChildState)
{
    TopState = StartingParentState;
    ChildState = StartingChildState;
    State.Current->Transition(StartingChildState);
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

EVerbType UCurrentCommand::GetVerbType() const
{
    TOptional<EVerbType> Verb = State.Current->GetVerb();
    if (auto HaveVerb = Verb.GetPtrOrNull())
    {
        return *HaveVerb;
    }
    return EVerbType::WalkTo;
}

void UCurrentCommand::ClickOnItem(TSharedRef<UInventoryItem> InventoryItem)
{
    if (State.Current->CanTransition(GLook_At_Item))
    {
        State.Transition(GLook_At_Item);
    }
    else if (State.Current->GetCode() == ECommandCodes::Pending)
    {
        
    }
#if WITH_EDITOR
    else
    {
        const FString CurrentDesc = State.Current->Description();
        UE_LOG(LogAdventureGame, Verbose, TEXT("Cannot walk to location from state %s"), *CurrentDesc);
    }
#endif
}

void UCurrentCommand::ClickOnHotSpot(TSharedRef<AHotSpot> HotSpot)
{
}

void UCurrentCommand::ClickOnSceneLocation()
{
    if (State.Current->CanTransition(GWalk_To_Location))
    {
        State.Transition(GWalk_To_Location);
    }
#if WITH_EDITOR
    else
    {
        const FString CurrentDesc = State.Current->Description();
        UE_LOG(LogAdventureGame, Verbose, TEXT("Cannot walk to location from state %s"), *CurrentDesc);
    }
#endif
}

void UCurrentCommand::UseItem(TSharedRef<UInventoryItem> InventoryItem)
{
    State.Transition(ECommandCodes::Active);
    State.Current->Transition(ECommandCodes::Use);
}

void UCurrentCommand::GiveItem(TSharedRef<UInventoryItem> InventoryItem)
{
}

void UCurrentCommand::UseItemOn(TSharedRef<UInventoryItem> InventoryItem)
{
}

void UCurrentCommand::GiveItemTo(TSharedRef<UInventoryItem> InventoryItem)
{
}

void UCurrentCommand::PerformVerb(EVerbType Verb)
{
    // clicked on a verb command
    const ECommandCodes Code = GetCommandCodeFromVerb(Verb);
    if (State.Current->CanTransition({ ECommandCodes::Pending, Code }))
    {
        State.Transition({ ECommandCodes::Pending, Code });
    }
}

void UCurrentCommand::Reset()
{
    TopState = StartingParentState;
    ChildState = StartingChildState;
    State = MakeParentStateMachine(TopState);
    State.Current->Transition(StartingChildState);
}
