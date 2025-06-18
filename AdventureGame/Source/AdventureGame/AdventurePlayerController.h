// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventureAIController.h"
#include "AdventureGameHUD.h"
#include "HotSpot.h"
#include "VerbType.h"
#include "Puck.h"
#include "GameFramework/PlayerController.h"
#include "AdventurePlayerController.generated.h"

DECLARE_DELEGATE(FRunInterruptedActionDelegate);

DECLARE_MULTICAST_DELEGATE(FBeginAction);
DECLARE_MULTICAST_DELEGATE(FInterruptAction);
DECLARE_MULTICAST_DELEGATE(FUpdateInteractionText);

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

	virtual void BeginPlay() override;
	
	void HandlePointAndClickInput();
	
	enum class ClickResult
	{
		HitHotspot,
		WalkToLocation,
		Rejected,
	};

	void MouseEnterHotSpot(AHotSpot *HotSpot);

	void MouseLeaveHotSpot();

	FVector2D LastMouseClick = FVector2D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	
	//////////////////////////////////
	///
	/// PLAYER CHARACTER AI
	///

	UFUNCTION()
	void HandleMovementComplete(EPathFollowingResult::Type Result);

	void WalkToLocation(FVector WorldLocation);

	void ClearCurrentPath();

	AAdventureAIController *GetAIController();

	//////////////////////////////////
	///
	/// VERBS AND INTERACTION
	/// 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	EVerbType CurrentVerb = EVerbType::WalkTo;

	void AssignVerb(EVerbType NewVerb);
	void PerformInteraction();
	void InterruptCurrentAction();

	FRunInterruptedActionDelegate RunInterruptedActionDelegate;
	FBeginAction BeginActionDelegate;
	FInterruptAction InterruptActionDelegate;
	FUpdateInteractionText UpdateInteractionTextDelegate;

	void TriggerBeginAction();
	void TriggerInterruptAction();
	void TriggerUpdateInteractionText();
	
	//////////////////////////////////
	///
	/// HOTSPOT INTERACTION
	///

private:
	/// Get the Hotspot under the click location, or pass through to walk location
	ClickResult GetClicked();

	void WalkToHotpot(AHotSpot *HotSpot);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TObjectPtr<AHotSpot> CurrentHotSpot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool HotspotInteraction = false;
	

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
	
	//////////////////////////////////
	///
	/// HUD WIDGET MANAGEMENT
	/// 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAdventureGameHUD *AdventureHUDWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool IsMouseOverUI = false;
};
