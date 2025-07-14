// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "CommandCodes.h"
#include "CommandState.h"
#include "AdventureGame/VerbType.h"
#include "UObject/Object.h"

#include "CurrentCommand.generated.h"

class UInventoryItem;
class AHotSpot;

class UCommandState;
/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UCurrentCommand : public UObject
{
    GENERATED_UCLASS_BODY()
public:
    UCurrentCommand(const ECommandCodes &StartingParentState, const ECommandCodes &StartingChildState);

    UCurrentCommand();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    ECommandCodes ChildState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    ECommandCodes TopState;
    
    FString GetCommandString() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Getter="GetCommandString", Category = "Command")
    FString CurrentState;
    
    void UseItem(UInventoryItem *Item);

    void GiveItem(UInventoryItem *Item);

    void WalkTo(AHotSpot* HotSpot);

    void WalkToLocation();

    void PerformVerb(EVerbType Verb);

    /// Jump back to the beginning state. No transition rules are checked.
    void Reset();
private:
    FTopLevelStateMachine State;
    ECommandCodes StartingParentState;
    ECommandCodes StartingChildState;
};
