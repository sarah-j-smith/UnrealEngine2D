// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventurePlayerController.h"
#include "Engine/TimerHandle.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "AdventureGameInstance.generated.h"

class ADoor;
class UAdventureGameHUD;

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UAdventureGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	//////////////////////////////////
	///
	/// EVENT HANDLERS
	///

	virtual void Init() override;
	
	//////////////////////////////////
	///
	/// INVENTORY
	///

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UItemList *Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UItemList> InventoryClass;

	//////////////////////////////////
	///
	/// DOOR MANAGEMENT
	///
	
	/// Call from a blueprint of a door to trigger loading the room.
	UFUNCTION(BlueprintCallable, Category="GameInstance")
	static void LoadRoom(ADoor *FromDoor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameInstance")
	TObjectPtr<ADoor> CurrentDoor;

	/// Set this value to the name of which ever level should be loaded at the game start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInstance")
	FName StartingLevelName = "TowerExterior"; 

	/// Set this value to the of which door the player should started at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInstance")
	FName StartingDoorLabel = "A1"; 

	//////////////////////////////////
	///
	/// TRANSITION CURRENT ROOM -> NEW ROOM VIA DOOR
	///

	/// Event to handle requests to load and transition from an old room
	/// to a new room based on the current door
	UFUNCTION(BlueprintCallable, Category="Room")
	void OnLoadRoom();

	/// Event for when a room is loaded
	UFUNCTION()
	void OnRoomLoaded();
	
	/// Event for when a room is unloaded
	UFUNCTION()
	void OnRoomUnloaded();

	void TriggerRoomTransition();

private:
	const FName OnLoadRoomName = "OnLoadRoom";
	const FName OnRoomLoadedName = "OnRoomLoaded";
	const FName OnRoomUnloadedName = "OnRoomUnloaded";
	enum class ERoomTransitionPhase
	{
		GameNotStarted,
		LoadStartingRoom,
		RoomCurrent,
		LoadNewRoom,
		NewRoomLoaded,
		UnloadOldRoom,
		RoomUnloaded,
		DelayProcessing,
	};
	
	ERoomTransitionPhase RoomTransitionPhase = ERoomTransitionPhase::GameNotStarted;

	// On game start:
	//  Member function called           State
	//     ------------                  ------------------
	//     OnLoadRoom()                  GameNotStarted
	//     LoadStartingRoom()            LoadStartingRoom
	//     OnRoomLoaded()                NewRoomLoaded
	//     NewRoomDelay()                DelayProcessing
	//     OnRoomLoadTimerTimeout()
	//     SetupRoom()
	//     StartNewRoom()                RoomCurrent
	//
	// On room transition to a different room:
	//  Member function called           State 
	//     ------------                  ------------------
	//     OnLoadRoom()                  RoomCurrent
	//     LoadRoom()                    LoadNewRoom
	//     OnRoomLoaded()                NewRoomLoaded
	//     UnloadRoom()                  UnloadOldRoom
	//     OnRoomUnloaded()
	//     NewRoomDelay()                DelayProcessing
	//     OnRoomLoadTimerTimeout()      
	//     SetupRoom()
	//     StartNewRoom()                RoomCurrent

	/// Load up the room specified by the current door
	void LoadRoom();

	/// Load up the room specified by the starting door
	void LoadStartingRoom();

	/// Unload the room specified by the current door
	void UnloadRoom();

	/// Run a short delay to allow scene to complete, and any geometry
	/// to be placed, spawned and stabilise.
	void NewRoomDelay();
	
	/// Setup the current room, find & load the door (move the player
	/// character to the relevant door). Hide the transition effect.
	/// Call this only after the delay to ensure the player character
	/// and all geometry is stable from any collisions.
	void SetupRoom();

	/// Restart play in the new room allowing input from the player.
	void StartNewRoom();

	void OnRoomLoadTimerTimeout();

	FLatentActionInfo GetLatentActionForHandler(FName EventName);
	
	FTimerHandle RoomLoadTimer;

	UPROPERTY(EditDefaultsOnly, Category="Room")
	float RoomLoadDelay = 0.5;

	//////////////////////////////////
	///
	/// UTILITIES
	///
	
	inline AAdventurePlayerController *GetAdventureController() const
	{
		APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		return Cast<AAdventurePlayerController>(PlayerController);
	}

	static UAdventureGameHUD *GetHUD();

	FName CurrentDoorLabel = FName("A1");
	
	ADoor *FindDoor(FName DoorLabel);

	void LoadDoor(const ADoor *Door);
private:
	FDelegateHandle OnInventoryChangedHandle;
};
