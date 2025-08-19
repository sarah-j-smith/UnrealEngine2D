// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "../Enums/PlayerCommand.h"
#include "../Enums/ChoiceState.h"

#include "AdventureAIController.h"
#include "../HUD/AdventureGameHUD.h"
#include "../HotSpots/HotSpot.h"
#include "../Enums/InteractionType.h"
#include "../Enums/VerbType.h"
#include "Puck.h"
#include "../Items/InventoryItem.h"
#include "AdventureGame/Constants.h"

#include "GameFramework/PlayerController.h"
#include "AdventurePlayerController.generated.h"

class UItemSlot;

DECLARE_DELEGATE(FRunInterruptedActionDelegate);

DECLARE_MULTICAST_DELEGATE(FBeginAction);
DECLARE_MULTICAST_DELEGATE(FInterruptAction);
DECLARE_MULTICAST_DELEGATE(FUpdateInteractionText);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FEndAction, EInteractionType /* Interaction */, int32 /* UID */, bool /* Completed */);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FEndBark, FText /* BarkText */, int32 /* UID */, bool /* Completed */);

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
	/// PLAYER BARK
	///
	
	void PlayerBark(const FText &BarkText, TOptional<FColor> TextColor = TOptional<FColor>(),
		float TimeToPause = 0, USphereComponent *Position = nullptr, int32 BarkTaskUid = 0);

	void PlayerBarkLines(const TArray<FText> &BarkTextArray, TOptional<FColor> TextColor = TOptional<FColor>(),
		float TimeToPause = 0, USphereComponent *Position = nullptr, int32 BarkTaskUid = 0);

	void ClearBark();

	/// This flag is true when the Bark Timer is running, and false otherwise.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBarking = false;

	FEndBark EndBark;

private:
	FText CurrentBarkText;
	int32 CurrentBarkTask;
	FTimerHandle TimerHandle_Bark;
	FTimerDelegate BarkTimerDelegate;

	UFUNCTION()
	void OnBarkTimerTimeOut();

public:
	
	//////////////////////////////////
	///
	/// EVENT HANDLERS
	///

#if WITH_EDITORONLY_DATA
	/// For game debugging, teleport to locations instead of walking there.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TeleportInsteadOfWalk = false;
#endif
	
	FEndAction EndAction;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Save Game")
	FString DefaultSaveGameName = SAVE_GAME_NAME;

	UFUNCTION(BlueprintCallable, Category="Save Game")
	void HandleSaveGame(const FString& GameName);

	UFUNCTION(BlueprintCallable, Category="Save Game")
	void HandleLoadGame(const FString& GameName);

	void HandleTouchInput(float X, float Y);
	
	void HandlePointAndClickInput();

	void HandleHotSpotClicked(AHotSpot* HotSpot);

	void HandleLocationClicked(const FVector& Location);

	void MouseEnterHotSpot(AHotSpot *HotSpot);

	void MouseLeaveHotSpot();

	void SetInputLocked(bool bLocked);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Getter="IsInputLocked", Category = UI)
	bool bInputLocked;
	
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
	void HandleAIMovementCompleteNotify(EPathFollowingResult::Type Result);

	bool ShouldCompleteMovementNextTick = false;

	UFUNCTION()
	void HandleMovementComplete();

	void WalkToLocation(const FVector &Location);

	void ClearCurrentPath();

	EAIMoveResult LastPathResult = EAIMoveResult::Unknown;
	
	void TeleportToLocation(const FVector &Location);
	
	void SetVerbAndCommandFromHotSpot(AHotSpot *HotSpot);

	void StopAIMovement();
	
	//////////////////////////////////
	///
	/// ITEMS AND INTERACTION
	///

public:
	UFUNCTION(BlueprintCallable, Category = "Items")
	void SwapSourceAndTarget();
	
	/// Which item will be the <b>subject</b> of the current verb eg "Open Box"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Commands")	
	UInventoryItem *SourceItem;

	/// Clears the <code>SourceItem</code> to null and sets it unlocked.
	UFUNCTION(Blueprintable)
	void ClearSourceItem()
	{
		SourceLocked = EChoiceState::Unlocked;
		SourceItem = nullptr;
	}

	/// Which item will the <b>object</b> of the current verb for
	/// example the door in "Use key on door"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	UInventoryItem *TargetItem;
	
	/// Clears the <code>SourceItem</code> to null and sets it unlocked.
	UFUNCTION(Blueprintable)
	void ClearTargetItem()
	{
		TargetLocked = EChoiceState::Unlocked;
		TargetItem = nullptr;
	}

	/// Whether the subject of the verb is locked. Locked item choices won't change
	/// on mouse over of inventory. Default is <code>Unlocked</code>.
	///
	/// Set to locked when an item is clicked as part of a command. Set to unlocked
	/// when Source is set to null.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Items")	
	EChoiceState SourceLocked = EChoiceState::Unlocked;

	/// Whether the target of the verb is locked. Locked item choices won't change
	/// on mouse over of inventory. Default is <code>Unlocked</code>.
	///
	/// Set to locked when a target item is clicked as part of a Use or Give
	/// command. Set to unlocked when Target is set to null.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	EChoiceState TargetLocked = EChoiceState::Unlocked;
	
	UInventoryItem *ItemAddToInventory(const EItemKind &ItemToAdd);

	void ItemRemoveFromInventory(const EItemKind &ItemToRemove);

	void ItemsRemoveFromInventory(const TSet<EItemKind> &ItemsToRemove);

	/// Handle a mouse click on an item button.
	void HandleInventoryItemClicked(UItemSlot *ItemSlot);
	
	void MouseEnterInventoryItem(UItemSlot *ItemSlot);

	void MouseLeaveInventoryItem();

	/// Perform an interaction on the target item, ie via use or give.
	void PerformItemInteraction();

	void PerformItemAction();

	void PerformInstantAction();

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

	/// Current verb action selected by the player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	EVerbType CurrentVerb = EVerbType::WalkTo;

	/// Item slot in the inventory either hovered or clicked.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	UItemSlot *CurrentItemSlot;

	/// Current hovered or clicked Hotspot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	TObjectPtr<AHotSpot> CurrentHotSpot;

	/// Location that the player is being sent to by a click
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Commands")
	FVector CurrentTargetLocation = FVector::ZeroVector;

	bool ShouldHighlightInteractionText() const
	{
		return CurrentCommand == EPlayerCommand::Active || CurrentCommand == EPlayerCommand::InstantActive;
	}

private:
	bool CanBrowseHotspot()
	{
		switch (CurrentCommand)
		{
		case EPlayerCommand::None:
		case EPlayerCommand::Hover:
		case EPlayerCommand::Targeting:
		case EPlayerCommand::UsePending:
		case EPlayerCommand::VerbPending:
			return true;
		default:
			return false;
		}
	}

	bool CanBrowseTarget()
	{
		return CurrentCommand == EPlayerCommand::Targeting;
	}

	bool CanBrowseSource()
	{
		switch (CurrentCommand)
		{
		case EPlayerCommand::None:
		case EPlayerCommand::Hover:
		case EPlayerCommand::VerbPending:
		case EPlayerCommand::UsePending:
		case EPlayerCommand::GivePending:
			return true;
		default:
			return false;
		}
	}

	
	UItemList *GetInventoryItemList();

	//////////////////////////////////
	///
	/// VERBS AND INTERACTION
	///
	
	void PerformHotSpotInteraction();

	/// Tell the UI to highlight and lock the action text
	/// Locked text won't change if the player hovers the mouse over other hotspots
	/// which is useful when the character is already obeying a previous command.
	void TriggerBeginAction();

	/// Clear text and clear the active button in the UI. Clearing the text also
	/// unlocks it and unhighlights it.
	void TriggerInterruptAction();

	/// Tell the UI to put the current verb and any current hotspot into the text display
	/// and if the HotspotInteraction is true, highlight the text.
	UFUNCTION(BlueprintCallable, Category="Actions")
	void TriggerUpdateInteractionText();

	/// Maybe use an FSM - https://gist.github.com/JoshLmao/a71e10d70c88b1c23418f20bcab26977
	/// ....or a HSM - see readme for State Trees.

public:
	UFUNCTION(BlueprintCallable, Category="Actions")
	void AssignVerb(EVerbType NewVerb);

	/// Stops any current action, items and hotspots, clearing the status
	UFUNCTION(BlueprintCallable, Category="Verb", DisplayName="ClearAction")
	void InterruptCurrentAction();

	UPROPERTY(BlueprintReadWrite, Category="Actions")
	bool ShouldInterruptCurrentActionOnNextTick = false;

	FRunInterruptedActionDelegate RunInterruptedActionDelegate;
	FBeginAction BeginActionDelegate;
	FInterruptAction InterruptActionDelegate;
	FUpdateInteractionText UpdateInteractionTextDelegate;
	FUpdateInteractionText UpdateInventoryTextDelegate;
	
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

	/// Get the Hotspot under the tap location, or null if no hotspot was found
	AHotSpot *HotSpotTapped(float LocationX, float LocationY);

	/// Invoke AI to move the character to the HotSpot's MoveToLocation
	void WalkToHotSpot(AHotSpot *HotSpot);

public:
	
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
