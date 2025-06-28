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
#include "AdventurePlayerController.generated.h"

class UItemSlot;
DECLARE_DELEGATE(FRunInterruptedActionDelegate);

DECLARE_MULTICAST_DELEGATE(FBeginAction);
DECLARE_MULTICAST_DELEGATE(FInterruptAction);
DECLARE_MULTICAST_DELEGATE(FUpdateInteractionText);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FEndAction, EInteractionType /* Interaction */, int32 /* UID */, bool /* Completed */);

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
	void HandleInventoryItemClicked(UItemSlot *ItemSlot);
	
	void MouseEnterInventoryItem(UItemSlot *ItemSlot);

	void MouseLeaveInventoryItem();

	void PerformItemInteraction(UInventoryItem *InventoryItem);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TObjectPtr<UInventoryItem> CurrentItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	bool ItemInteraction = false;

	//////////////////////////////////
	///
	/// VERBS AND INTERACTION
	///
	
private:
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	EVerbType CurrentVerb = EVerbType::WalkTo;

	void AssignVerb(EVerbType NewVerb);
	void InterruptCurrentAction();

	FRunInterruptedActionDelegate RunInterruptedActionDelegate;
	FBeginAction BeginActionDelegate;
	FInterruptAction InterruptActionDelegate;
	FUpdateInteractionText UpdateInteractionTextDelegate;

	UFUNCTION(BlueprintCallable)
	void PlayerBark(FText BarkText);
	
	void PlayerClimb(int32 UID);
	
	void PlayerInteract(int32 UID);
	
	void PlayerSit(int32 UID);

private:
	UFUNCTION()
	void OnPlayerInteractComplete(bool Complete);
	int32 PlayerInteractUID = 0;
	
	UFUNCTION()
	void OnPlayerSitComplete(bool Complete);
	int32 PlayerSitUID = 0;

	UFUNCTION()
	void OnPlayerClimbComplete(bool Complete);
	int32 PlayerClimbUID = 0;

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
	TObjectPtr<AAdventureCharacter> PlayerCharacter;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsMouseOverUI = false;
};
