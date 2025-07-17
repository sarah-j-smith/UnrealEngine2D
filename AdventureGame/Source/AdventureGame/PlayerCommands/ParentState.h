#pragma once

#include "CommandCodes.h"
#include "CommandState.h"
#include "ICommandState.h"
#include "CommandStateMachine.h"

template <ECommandCodes E>
class TFParentState : public TFCommandState<E>, public ICommandState
{
public:
    /// Default constructor which sets the starting state to Initial
    TFParentState();

    virtual ~TFParentState() override = default;

    virtual ECommandCodes GetCode() const override { return E; }
    
    DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCommandChangedSignature, FName /* From */, FName /* Child */);
    FOnCommandChangedSignature OnCommandChanged;

    virtual void ClickOnItem(UInventoryItem *Item);

    virtual void ClickOnHotSpot(AHotSpot *HotSpot);
    
    virtual void Transition(ECommandCodes Code) override;
    
    virtual ECommandCodes CurrentChildState() const;

    virtual FString Description() const override;
    
protected:
    // Pointer to my current child state, or null if in an initial or terminal state.
    TSharedPtr<FCommandStateMachine> State;
};

//////////////////////////////////
///
///  PARENT STATE - TEMPLATE IMPLEMENTATIONS
///

template <ECommandCodes E>
TFParentState<E>::TFParentState()
    : State(new FCommandStateMachine())
{
    State->SetName(FName(CommandCodesToString(E) + TEXT("State Machine")));
}

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
    State->Transition(Code);
    const FName NewCode(CommandCodesToString(CurrentChildState()));
    OnCommandChanged.Broadcast(OldCode, NewCode);
}

template <ECommandCodes E>
ECommandCodes TFParentState<E>::CurrentChildState() const
{
    return State->IsInitial() ? ECommandCodes::Initial : State->GetCurrentState()->GetCode();
}

template <ECommandCodes E>
FString TFParentState<E>::Description() const
{
    const FString ParentString = CommandCodesToString(GetCode());
    const FString ChildString = CommandCodesToString(CurrentChildState());
    return FString::Printf(TEXT("%s :: %s"), *ParentString, *ChildString);
}
