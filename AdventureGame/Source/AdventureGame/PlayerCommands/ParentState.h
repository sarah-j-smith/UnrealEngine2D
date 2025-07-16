#pragma once
#include "CommandCodes.h"

#include "CommandState.h"

template <ECommandCodes E>
class TFParentState : public TFCommandState<E>, public ICommandState
{
public:
    TFParentState() = default;

    explicit TFParentState(const ECommandCodes &StartingCode)
        : State(MakeParentStateMachine(StartingCode))
    {}

    virtual ~TFParentState() override = default;

    virtual ECommandCodes GetCode() const override { return E; }
    
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCommandChangedSignature, FName /* From */, FName /* Child */);
    FOnCommandChangedSignature OnCommandChanged;

    virtual void ClickOnItem(UInventoryItem *Item);

    virtual void ClickOnHotSpot(AHotSpot *HotSpot);
    
    virtual void Transition(ECommandCodes Code) override;
    
    virtual ECommandCodes CurrentChildState() const
    {
        return State.Current->GetCode();
    }

    virtual FString Description() const override;
    
protected:
    FCommandStateMachine State;
};

//////////////////////////////////
///
///  PARENT STATE - TEMPLATE IMPLEMENTATIONS
///

template <ECommandCodes E>
void TFParentState<E>::ClickOnItem(UInventoryItem* Item)
{
    UE_LOG(LogTemp, Warning, TEXT("%s should not call TFParentState::ClickOnItem"),
        *(Description()));
}

template <ECommandCodes E>
void TFParentState<E>::ClickOnHotSpot(AHotSpot* HotSpot)
{
    UE_LOG(LogTemp, Warning, TEXT("%s should not call TFParentState::ClickOnItem"),
        *(Description()));
}

template <ECommandCodes E>
void TFParentState<E>::Transition(ECommandCodes Code)
{
    const FName OldCode(CommandCodesToString(CurrentChildState()));
    State.Transition(Code);
    State.Current->SetParent(MakeShareable(this));
    const FName NewCode(CommandCodesToString(CurrentChildState()));
    OnCommandChanged.Broadcast(OldCode, NewCode);
}

template <ECommandCodes E>
FString TFParentState<E>::Description() const
{
    const FString ParentString = CommandCodesToString(GetCode());
    const FString ChildString = CommandCodesToString(CurrentChildState());
    return FString::Printf(TEXT("%s :: %s"), *ParentString, *ChildString);
}

DECLARE_STATE_CLASS(Initial, Initial)

DECLARE_STATE_CLASS(Terminal, Terminal)

