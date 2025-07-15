// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "AdventureAIController.h"
#include "AdventureGameHUD.h"
#include "HotSpot.h"
#include "InteractionType.h"
#include "VerbType.h"
#include "Puck.h"
#include "InventoryItem.h"

#include "GameFramework/PlayerController.h"
#include "PlayerCommands/CurrentCommand.h"
#include "AdventurePlayerController.generated.h"

class UItemSlot;

DECLARE_DELEGATE(FRunInterruptedActionDelegate);

DECLARE_MULTICAST_DELEGATE(FBeginAction);
DECLARE_MULTICAST_DELEGATE(FInterruptAction);
DECLARE_MULTICAST_DELEGATE(FUpdateInteractionText);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FEndAction, EInteractionType /* Interaction */, int32 /* UID */, bool /* Completed */);

/// What commands has the player current issued in the UX.
/// Note that sometimes the game will default the CurrentAction if the player
/// has not specifically clicked on a command verb, eg WalkTo if over the game
/// window, and LookAt if over the inventory. These states are for when the player
/// has specifically given a command.
UENUM(BlueprintType)
enum class EPlayerCommand : uint8
{
	/// The player has not clicked to activate any command
	None = 0           UMETA(DisplayName = "NONE"),

	/// The player has activated by clicking on the CurrentVerb
	VerbActivated = 1  UMETA(DisplayName = "VERB_ACTIVATED"),

	/// A verb was activated (implicitly or explicitly) and now is being actioned
	/// because the player clicked on a target for the verb
	VerbInProgress = 2 UMETA(DisplayName = "VERB_INPROGRESS"),
};

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API AAdventurePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAdventurePlayerController();

	//////////////////////////////////
	///
	/// COMMAND STATE
	///

	/// Parent command state in HSM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Getter="GetSourceInventoryItem", Category="Command State")	
	UInventoryItem *SourceInventoryItem;
	
	/// Parent command state in HSM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Getter="GetRootCommandCode", Category="Command State")	
	ECommandCodes RootCommandCode;

	/// Parent command state in HSM
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Getter="GetLeafCommandCode", Category="Command State")	
	ECommandCodes LeafCommandCode;

	UInventoryItem *GetSourceInventoryItem() const
	{
		return Command->SourceItem;
	}
	
	ECommandCodes GetRootCommandCode() const
	{
		return Command->TopState;
	}
	
	ECommandCodes GetLeafCommandCode() const
	{
		return Command->ChildState;
	}
	
	UPROPERTY()
	UCurrentCommand *Command;

	//////////////////////////////////
	///
	/// EVENT HANDLERS
	///
	
	FEndAction EndAction;
	
	virtual void BeginPlay() override;
	
	void HandlePointAndClickInput();

	void MouseEnterHotSpot(AHotSpot *HotSpot);

	void MouseLeaveHotSpot();

	void SetInputLocked(bool bLocked);

	bool IsInputLocked() const;

	bool IsAlreadyAtHotspotClicked() const
	{
		return AIStatus == EAIStatus::AlreadyThere;
	}

private:
	bool GetMouseClickPosition(float &LocationX, float &LocationY);

	FVector2D LastMouseClick = FVector2D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

	/// At certain times, eg level loading, do not allow any user input.
	bool LockInput = false;
	
	//////////////////////////////////
	///
	/// PLAYER CHARACTER AI
	///
	
	enum class EAIStatus:uint8
	{
		Idle,
		MakingRequest,
		AlreadyThere,
		Moving,
		Done
	};

	EAIStatus AIStatus = EAIStatus::Idle;
	
	enum class EAIMoveResult
	{
		Unknown,
		Moving,
		Success,
		Fail
	};

	UFUNCTION()
	void HandleMovementComplete();

	void WalkToLocation(const FVector &Location);

	void ClearCurrentPath();

	UFUNCTION()
	void HandleAIMovementCompleteNotify(EPathFollowingResult::Type Result);

	FTimerDelegate MovementCompleteTimerDelegate;

	EAIMoveResult LastPathResult = EAIMoveResult::Unknown;
	
	//////////////////////////////////
	///
	/// ITEMS AND INTERACTION
	///

public:
	UInventoryItem *ItemAddToInventory(const EItemKind &ItemToAdd, FText Description = FText::GetEmpty());

	void ItemRemoveFromInventory(const EItemKind &ItemToRemove);

	/// Handle a mouse click on an item button.
	void HandleInventoryItemClicked(UItemSlot *ItemSlot);
	
	void MouseEnterInventoryItem(UItemSlot *ItemSlot);

	void MouseLeaveInventoryItem();

	/// Perform an interaction on an item.
	void PerformItemInteraction(const UInventoryItem *InventoryItem);

	void UseAnItem(const EItemKind ItemToUse)
	{
		ActiveItem = ItemToUse;
		IsUsingItem = true;
		CurrentVerb = EVerbType::UseItem;
		TriggerUpdateInventoryText();
	}

	void GiveAnItem(const EItemKind ItemToGive)
	{
		ActiveItem = ItemToGive;
		IsUsingItem = true;
		CurrentVerb = EVerbType::GiveItem;
		TriggerUpdateInventoryText();
	}

	UFUNCTION(BlueprintCallable, Category="Items")
	void CombineItems(const UInventoryItem *InventoryItemSource,
		const UInventoryItem *InventoryItemToCombineWith,
		EItemKind ResultingItem, FText TextToBark, FText ResultDescription = FText::GetEmpty());
	
	/// Tell the UI to put the current verb and any current inventory item into the text display
	/// and if the InventoryItemInteraction is true, highlight the text.
	void TriggerUpdateInventoryText();

	//////////////////////////////////
	///
	/// COMMAND STATE
	///
	
	/// Has the player currently issued a command?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	EPlayerCommand CurrentCommand = EPlayerCommand::None;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	EVerbType CurrentVerb = EVerbType::WalkTo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	const UInventoryItem *CurrentItem;

	/// True if the player character is currently interacting with an item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	bool ItemInteraction = false;

	/// True if the player character is currently using via the "Use" verb on an item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	bool IsUsingItem = false;

	/// True if the player character is currently giving via the "Give" verb on an item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	bool IsGivingItem = false;

	/// An item current in process of "Use" or "Give"
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	EItemKind ActiveItem = EItemKind::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	UItemSlot *CurrentItemSlot;

private:
	UItemList *GetInventoryItemList();

	//////////////////////////////////
	///
	/// VERBS AND INTERACTION
	///
	
	void PerformInteraction();

	/// Tell the UI to highlight and lock the action text
	/// Locked text won't change if the player hovers the mouse over other hotspots
	/// which is useful when the character is already obeying a previous command.
	void TriggerBeginAction();

	/// Clear text and clear the active button in the UI. Clearing the text also
	/// unlocks it and unhighlights it.
	void TriggerInterruptAction();

	/// Tell the UI to put the current verb and any current hotspot into the text display
	/// and if the HotspotInteraction is true, highlight the text.
	void TriggerUpdateInteractionText();

	/// Maybe use an FSM - https://gist.github.com/JoshLmao/a71e10d70c88b1c23418f20bcab26977
	/// ....or a HSM - see readme for State Trees.

public:
	void AssignVerb(EVerbType NewVerb);

	void HoverVerb(TOptional<EVerbType> NewVerb);

	/// Stops any current action, items and hotspots, clearing the status
	UFUNCTION(BlueprintCallable, Category="Verb", DisplayName="ClearAction")
	void InterruptCurrentAction();

	FRunInterruptedActionDelegate RunInterruptedActionDelegate;
	FBeginAction BeginActionDelegate;
	FInterruptAction InterruptActionDelegate;
	FUpdateInteractionText UpdateInteractionTextDelegate;
	FUpdateInteractionText UpdateInventoryTextDelegate;

	UFUNCTION(BlueprintCallable)
	void PlayerBark(FText BarkText);
	
	void PlayerClimb(int32 UID, EInteractTimeDirection InteractDirection);
	
	void PlayerInteract(int32 UID, EInteractTimeDirection InteractDirection);
	
	void PlayerSit(int32 UID, EInteractTimeDirection InteractDirection);

	/// Turn from front facing to the left, or left facing to the front if reversed.
	void PlayerTurnLeft(int32 UID, EInteractTimeDirection InteractDirection);

	/// Turn from front facing to the right, or right facing to the front if reversed.
	void PlayerTurnRight(int32 UID, EInteractTimeDirection InteractDirection);
private:
	UFUNCTION()
	void OnPlayerAnimationComplete(EInteractionType Interaction, bool Complete);
	
	UFUNCTION()
	void OnPlayerInteractComplete(bool Complete);
	int32 PlayerInteractUID = 0;
	
	UFUNCTION()
	void OnPlayerSitComplete(bool Complete);
	int32 PlayerSitUID = 0;

	UFUNCTION()
	void OnPlayerClimbComplete(bool Complete);
	int32 PlayerClimbUID = 0;

	UFUNCTION()
	void OnPlayerTurnLeftComplete(bool Complete);

	UFUNCTION()
	void OnPlayerTurnRightComplete(bool Complete);
	int32 PlayerTurnUID = 0;

	void EndTaskAction(EInteractionType InteractionType, int32 UID, bool Complete);
	
	//////////////////////////////////
	///
	/// HOTSPOT INTERACTION
	///
	
	/// Get the Hotspot under the click location, or null if no hotspot was found
	AHotSpot *HotSpotClicked();

	/// Invoke AI to move the character to the HotSpot's MoveToLocation
	void WalkToHotSpot(AHotSpot *HotSpot);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TObjectPtr<AHotSpot> CurrentHotSpot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool HotspotInteraction = false;

	/// When the character is performing a sequence of chained actions do not allow any input
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsPerformingTaskInteraction = false;
	
	//////////////////////////////////
	///
	/// INITIALISATION
	///

	/// The current `AAdventureCharacter` managed by this adventure controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AAdventureCharacter *PlayerCharacter;

	/// An object of this class will be spawned and will be controlled by the AI pathfinder
	/// then the player will follow this puck. This is needed because the player character cannot
	/// receive input whilst possessed by the AI.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<APuck> PuckClassToSpawn;
	
	APawn *SetupPuck(AAdventureCharacter *PlayerCharacter);

	void SetupAIController(APawn *AttachToPawn);
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAdventureGameHUD> AdventureHUDClass;

	void SetupHUD();

	void SetupAnimationDelegates();
	
	//////////////////////////////////
	///
	/// HUD WIDGET MANAGEMENT
	/// 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAdventureGameHUD *AdventureHUDWidget;

	void SetMouseOverUI(bool IsMouseOverUI)
	{
		if (IsMouseOverUI == this->IsMouseOverUI) return;
		UpdateMouseOverUI(IsMouseOverUI);
	}
	
private:
	bool IsMouseOverUI = false;

	void UpdateMouseOverUI(bool NewMouseIsOverUI);

	friend class FInventoryCustomisation;
};
