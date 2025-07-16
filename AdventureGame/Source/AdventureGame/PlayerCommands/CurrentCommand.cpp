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

UCurrentCommand::UCurrentCommand(FObjectInitializer const& Initializer)
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
    if (const auto HaveVerb = Verb.GetPtrOrNull())
    {
        return *HaveVerb;
    }
    return EVerbType::WalkTo;
}

void UCurrentCommand::SetHoverState(ECommandCodes HoverState)
{
    if (IsHoverCommandCode(HoverState))
    {
        switch (HoverState)
        {
        case ECommandCodes::HoverScene:
        case ECommandCodes::HoverInventory:
        case ECommandCodes::HoverVerb:
            if (State.Current->CanTransition({ECommandCodes::Free, HoverState}))
            {
                State.Transition({ECommandCodes::Free, HoverState});
            }
            break;
        default:
            // Should use HoverItem or HoverHotSpot o those - we need to capture a pointer to the item/hotspot
            UE_LOG(LogAdventureGame, Error, TEXT("Use dedicated hover function to for %s"),
                   *CommandCodesToString(HoverState));
        }
    }
#if WITH_EDITOR
    else
    {
        UE_LOG(LogAdventureGame, Error, TEXT("SetHoverState called with non-hover state %s"),
               *CommandCodesToString(HoverState));
    }
#endif
}

void UCurrentCommand::SetHoverItem(UInventoryItem* InventoryItem)
{
    if (InventoryItem)
    {
        if (State.Current->CanTransition({ECommandCodes::Free, ECommandCodes::HoverItem}))
        {
            State.Transition({ECommandCodes::Free, ECommandCodes::HoverItem});
            SourceItem = InventoryItem;
        }
    }
    else
    {
        if (State.Current->CanTransition({ECommandCodes::Free, ECommandCodes::HoverItem}))
        {
            State.Transition({ECommandCodes::Free, ECommandCodes::HoverInventory});
            SourceItem = nullptr;
        }
    }
}

void UCurrentCommand::SetHoverHotSpot(AHotSpot* HotSpot)
{
    if (HotSpot)
    {
        if (State.Current->CanTransition({ECommandCodes::Free, ECommandCodes::HoverItem}))
        {
            State.Transition({ECommandCodes::Free, ECommandCodes::HoverItem});
            TargetHotSpot = HotSpot;
        }
    }
    else
    {
        if (State.Current->CanTransition({ECommandCodes::Free, ECommandCodes::HoverItem}))
        {
            State.Transition({ECommandCodes::Free, ECommandCodes::HoverItem});
            TargetHotSpot = nullptr;
        }
    }
}

void UCurrentCommand::ClickOnItem(TSharedRef<UInventoryItem> InventoryItem)
{
    if (State.Current->CanTransition(GLook_At_Item))
    {
        State.Transition(GLook_At_Item);
        return;
    }
    auto Verb = State.Current->GetVerb();
    if (EVerbType* VerbCode = Verb.GetPtrOrNull())
    {
        const auto VerbCommand = GetCommandCodeFromVerb(*VerbCode);
        if (const FStatePath VerbActive = {ECommandCodes::Active, VerbCommand}; State.Current->
            CanTransition(VerbActive))
        {
            State.Transition(VerbActive);
        }
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
    if (State.Current->CanTransition({ECommandCodes::Pending, Code}))
    {
        State.Transition({ECommandCodes::Pending, Code});
    }
}

void UCurrentCommand::Reset()
{
    TopState = StartingParentState;
    ChildState = StartingChildState;
    State = MakeParentStateMachine(TopState);
    State.Current->Transition(StartingChildState);
}

void UCurrentCommand::Transition(FStatePath DestinationPath)
{
    TopState = DestinationPath.Parent;
    ChildState = DestinationPath.Child;
    State.Transition(DestinationPath);
}

void UCurrentCommand::Transition(ECommandCodes DestinationState)
{
    State.Current->Transition(DestinationState);
    TopState = DestinationState;
}
