// (c) 2025 Sarah Smith


#include "CurrentCommand.h"
#include "ICommandState.h"
#include "TargetingTopState.h"
#include "AdventureGame/AdventureGame.h"

UCurrentCommand::UCurrentCommand()
    : State(new FCommandStateMachine())
{
    State->SetName("Top Level");
    UE_LOG(LogAdventureGame, Verbose, TEXT("UCurrentCommand constructed via default constructor"));
}

ECommandCodes UCurrentCommand::GetChildStateCode() const
{
    return State->IsInitial() ? ECommandCodes::Initial : State->GetCurrentState()->GetCode();
}

ECommandCodes UCurrentCommand::GetTopStateCode() const
{
    return State->GetCommandCode();
}

UCurrentCommand::UCurrentCommand(const FObjectInitializer& Initializer)
    : UCurrentCommand()
{
    UE_LOG(LogAdventureGame, Verbose, TEXT("UCurrentCommand constructed via FObjectInitializer"));
}

FString UCurrentCommand::GetCommandString() const
{
    return FString::Printf(TEXT("%s::%s"), *CommandCodesToString(TopState), *CommandCodesToString(ChildState));
}

EVerbType UCurrentCommand::GetVerbType() const
{
    TOptional<EVerbType> Verb = State->GetCurrentVerb();
    if (const auto HaveVerb = Verb.GetPtrOrNull())
    {
        return *HaveVerb;
    }
    return EVerbType::WalkTo;
}

bool UCurrentCommand::ValidateInitial(const ECommandCodes InitialState)
{
    if (State->IsInitial())
    {
        if (InitialState == ECommandCodes::HoverScene)
        {
            return true;
        }
#if WITH_EDITOR
        UE_LOG(LogAdventureGame, Error, TEXT("Illegal initial transition -> %s"),
               *CommandCodesToString(InitialState));
#endif
        return false;
    }
    return true;
}

bool UCurrentCommand::ValidateHoverState(ECommandCodes HoverState)
{
    if (!ValidateInitial(HoverState))
    {
        return false;
    }
    if (!IsHoverCommandCode(HoverState))
    {
#if WITH_EDITOR
        UE_LOG(LogAdventureGame, Error, TEXT("SetHoverState called with non-hover state %s"),
               *CommandCodesToString(HoverState));
#endif
        return false;
    }
    if (HoverState == ECommandCodes::HoverItem || HoverState == ECommandCodes::HoverHotSpot)
    {
#if WITH_EDITOR
        // Should use HoverItem or HoverHotSpot for those - we need to capture a pointer to the item/hotspot
        UE_LOG(LogAdventureGame, Error, TEXT("Use dedicated hover function to for %s"),
               *CommandCodesToString(HoverState));
#endif
        return false;
    }
    return true;
}

void UCurrentCommand::SetHoverState(ECommandCodes HoverState)
{
    const FStatePath HoverPath = {ECommandCodes::Free, HoverState};
    if (ValidateHoverState(HoverState))
    {
        switch (HoverState)
        {
        case ECommandCodes::HoverScene:
        case ECommandCodes::HoverInventory:
        case ECommandCodes::HoverVerb:
            if (State->IsInitial() || State->GetCurrentState()->CanTransition(HoverPath))
            {
                State->Transition(HoverPath);
            }
            break;
        default:
            break;
        }
    }
}

void UCurrentCommand::SetHoverItem(UInventoryItem* InventoryItem)
{
    if (InventoryItem)
    {
        // Mouse moved from the inventory UI around the items, into a populated item button
        if (ValidateInitial(GHover_Item.Child) && State->GetCurrentState()->CanTransition(GHover_Item))
        {
            State->Transition(GHover_Item);
            SourceItem = InventoryItem;
        }
    }
    else
    {
        // Mouse moved from a populated item button, into the inventory UI around the items
        if (ValidateInitial(GHover_Inventory.Child) && State->GetCurrentState()->CanTransition(GHover_Inventory))
        {
            State->Transition(GHover_Inventory);
            SourceItem = nullptr;
        }
    }
}

void UCurrentCommand::SetHoverHotSpot(AHotSpot* HotSpot)
{
    if (HotSpot)
    {
        if (ValidateInitial(GHover_HotSpot.Child) && State->GetCurrentState()->CanTransition(GHover_HotSpot))
        {
            State->Transition(GHover_HotSpot);
            TargetHotSpot = HotSpot;
        }
    }
    else
    {
        if (ValidateInitial(GHover_Scene.Child) && State->GetCurrentState()->CanTransition(GHover_Scene))
        {
            State->Transition(GHover_Scene);
            TargetHotSpot = nullptr;
        }
    }
}

bool UCurrentCommand::ValidateClickTransition(ECommandCodes UseTransition)
{
    return true;
    //     auto Verb = State->GetCurrentVerb();
    //     ECommandCodes VerbCommand = ECommandCodes::Initial;
    //     if (EVerbType* VerbCode = Verb.GetPtrOrNull())
    //     {
    //         VerbCommand = GetCommandCodeFromVerb(*VerbCode);
    //     }
    //     switch (UseTransition)
    //     {
    //     case ECommandCodes::LookAtItem:
    //         if (!ValidateInitial(GLook_At_Item.Child)) return false;
    //         
    //     case ECommandCodes::UseOn:
    //     }
    //                 
    //     }
    //         if (*VerbCode == EVerbType::Use)
    //         {
    // #if WITH_EDITOR
    //     
    // #endif
    //         }
    //     }
}

void UCurrentCommand::ClickOnItem(TSharedRef<UInventoryItem> InventoryItem)
{
    if (State->GetCurrentState()->CanTransition(GTarget_UseOn))
    {
        SourceItem = InventoryItem.ToSharedPtr().Get();
        TargetItem = nullptr;
        ValidateClickTransition(ECommandCodes::UseOn);
        State->Transition(GTarget_UseOn);
    }
    else if (State->GetCurrentState()->CanTransition(GTarget_GiveTo))
    {
        SourceItem = InventoryItem.ToSharedPtr().Get();
        TargetItem = nullptr;
        SourceItem = InventoryItem.ToSharedPtr().Get();
        ValidateClickTransition(ECommandCodes::GiveTo);
        State->Transition(GTarget_GiveTo);
    }
    else if (State->GetCurrentState()->CanTransition(GActive_UseOn))
    {
        TargetItem = InventoryItem.ToSharedPtr().Get();
        ValidateClickTransition(ECommandCodes::UseOn);
        State->Transition(GActive_UseOn);
    }
    else if (State->GetCurrentState()->CanTransition(GActive_GiveTo))
    {
        TargetItem = InventoryItem.ToSharedPtr().Get();
        ValidateClickTransition(ECommandCodes::GiveTo);
        State->Transition(GActive_GiveTo);
    }
    else if (State->GetCurrentState()->CanTransition(GActive_GiveTo))
    {
    }
    else if (State->GetCurrentState()->CanTransition(GLook_At_Item))
    {
        SourceItem = InventoryItem.ToSharedPtr().Get();
        TargetItem = nullptr;
        ValidateClickTransition(ECommandCodes::LookAtItem);
        State->Transition(GLook_At_Item);
    }
}

void UCurrentCommand::ClickOnHotSpot(TSharedRef<AHotSpot> HotSpot)
{
}

void UCurrentCommand::ClickOnSceneLocation()
{
    if (State->GetCurrentState()->CanTransition(GWalk_To_Location))
    {
        State->Transition(GWalk_To_Location);
    }
#if WITH_EDITOR
    else
    {
        const FString CurrentDesc = State->GetCurrentState()->Description();
        UE_LOG(LogAdventureGame, Verbose, TEXT("Cannot walk to location from state %s"), *CurrentDesc);
    }
#endif
}

void UCurrentCommand::UseItem(TSharedRef<UInventoryItem> InventoryItem)
{
    State->Transition(ECommandCodes::Active);
    State->GetCurrentState()->Transition(ECommandCodes::Use);
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
    if (State->GetCurrentState()->CanTransition({ECommandCodes::Pending, Code}))
    {
        State->Transition({ECommandCodes::Pending, Code});
    }
}

void UCurrentCommand::Reset()
{
    State = MakeShared<FCommandStateMachine>();
}

void UCurrentCommand::Transition(FStatePath DestinationPath)
{
    State->Transition(DestinationPath);
}

void UCurrentCommand::Transition(ECommandCodes DestinationState)
{
    State->GetCurrentState()->Transition(DestinationState);
}
