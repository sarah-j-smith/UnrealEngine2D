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

    /// An initial item for an action, the primary item that is the target of a verb,
    /// eg the <i>Box</i> in "Open Box". This must be selected from the players inventory.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    UInventoryItem *SourceItem = nullptr;
    
    /// A secondary item for an action, the "act with" or held item that is the
    /// target of a use or give verb, eg the <i>Screwdriver</i> in "Use Screwdriver on box".
    /// This must be selected from the players inventory.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    UInventoryItem *TargetItem = nullptr;
    
    /// A target for an action, the "act on" or scene item that is the
    /// target of a verb, eg the <i>Door</i> in "Use Key on Door".
    /// This must be selected from the game scene.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command")
    AHotSpot *TargetHotSpot = nullptr;
    
    FString GetCommandString() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Getter="GetCommandString", Category = "Command")
    FString CurrentState;

    EVerbType GetVerbType() const;

    // 
    void SetHoverState(ECommandCodes HoverState);

    /// Set the currently hovered <code>InventoryItem</code> or if the
    /// @param InventoryItem is null, clear it.
    void SetHoverItem(UInventoryItem *InventoryItem);

    /// Set the currently hovered <code>HotSpot</code> or if the
    /// @param HotSpot is null, clear it.
    void SetHoverHotSpot(AHotSpot *HotSpot);

    /// Set the currently hovered <code>HotSpot</code> or if the
    /// @param VerbType optional is unset, clear it.
    void SetHoverVerb(TOptional<EVerbType> VerbType);

    void ClickOnItem(TSharedRef<UInventoryItem> InventoryItem);

    void ClickOnHotSpot(TSharedRef<AHotSpot> HotSpot);

    void ClickOnSceneLocation();
    
    void UseItem(TSharedRef<UInventoryItem> InventoryItem);

    void GiveItem(TSharedRef<UInventoryItem> InventoryItem);

    /// The player clicked on an item that can have another already
    /// selected item combined with it to give an effect.
    void UseItemOn(TSharedRef<UInventoryItem> InventoryItem);

    /// The player clicked on an item representing an entity who
    /// can be given an item that is already selected.
    void GiveItemTo(TSharedRef<UInventoryItem> InventoryItem);

    /// The player clicked on and selected a verb. Now it has to be targeted onto something.
    void PerformVerb(EVerbType Verb);

    /// Jump back to the beginning state. No transition rules are checked.
    void Reset();
private:
    FCommandStateMachine State;
    ECommandCodes StartingParentState;
    ECommandCodes StartingChildState;
};
