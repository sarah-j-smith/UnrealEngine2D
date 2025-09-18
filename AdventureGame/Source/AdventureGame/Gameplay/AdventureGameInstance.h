// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "DataSaveRecord.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"

#include "../AdventureGame.h"

#include "../Enums/RoomTransitionPhase.h"
#include "Engine/TimerHandle.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "AdventureGameInstance.generated.h"

class UItemList;
class AAdventurePlayerController;
class UAdventureSave;
class ADoor;
class UAdventureGameHUD;

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UAdventureGameInstance : public UGameInstance, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	//////////////////////////////////
	///
	/// EVENT HANDLERS
	///

	virtual void Init() override;

	UFUNCTION()
	void OnSaveHotSpot(AHotSpot* HotSpot);

	UFUNCTION()
	void OnLoadHotSpot(AHotSpot* HotSpot);
	
	//////////////////////////////////
	///
	/// INVENTORY
	///

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UItemList *Inventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UItemList> InventoryClass;

	/// All the tags currently set in the game
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Save Game")
	FGameplayTagContainer GameplayTags;
	
	// IGameplayTagAssetInterface interface.
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
private:
	void CreateInventory();
	

	//////////////////////////////////
	///
	/// DOOR MANAGEMENT
	///

public:
	
	/// Call from a blueprint of a door to trigger loading the room.
	UFUNCTION(BlueprintCallable, Category="GameInstance")
	static void LoadRoom(ADoor *FromDoor);

	/// The door which the player most recently walked through into the current room.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GameInstance")
	ADoor *CurrentDoor;

	/// Set this value to the name of which ever level should be loaded at the game start
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInstance")
	FName StartingLevelName = "TowerExterior"; 

	/// Set this value to the of which door the player should started at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GameInstance")
	FName StartingDoorLabel = "A1";

	/// TriggerRoomTransition to a room based on a door that has a new level name and door label
	void SetDestinationFromDoor(ADoor *NewCurrentDoor);

	/// TriggerRoomTransition to a room based on the level name and door label
	void SetLoadTarget(FName LevelName, FName DoorLabel);

	// The name of door the player character is at, or was most recently at.
	FName CurrentDoorLabel = FName("A1");

	// The name of the current lavel
	FName CurrentLevelName = FName("TowerExterior");

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

	/// Run the OnLoadRoom event to load a new level, and unload the current level.
	void TriggerRoomTransition();

	
	//////////////////////////////////
	///
	/// SAVE GAME
	///

	/// If true, when the game is launched a saved game will be checked for, and if it
	/// exists it will be loaded. Useful for debugging though.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameInstance")
	bool ShouldCheckForSaveGameOnLoad = false;
	
	/// Store current game state into the <code>CurrentSaveGame</code> object ready for it to
	/// be serialised to persistent storage with the given name.
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void SaveGame();

	/// Retrieve current game state from the <code>CurrentSaveGame</code> object and make it
	/// active in the current game.
	UFUNCTION(BlueprintCallable, Category="SaveGame")
	void LoadGame();

	void RegisterHotSpotForSaveAndLoad(AHotSpot *HotSpot);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	TSubclassOf<UAdventureSave> SaveGameClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
	UAdventureSave* CurrentSaveGame;
	
private:
	
	TArray< TWeakObjectPtr< AHotSpot >> RegisteredHotSpots;

	UPROPERTY()
	TArray<FDataSaveRecord> AdventureSaves;
	
	//////////////////////////////////
	///
	/// LOAD ROOM
	///
	
	const FName OnLoadRoomName = "OnLoadRoom";
	const FName OnRoomLoadedName = "OnRoomLoaded";
	const FName OnRoomUnloadedName = "OnRoomUnloaded";
	
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
	//
	// On Init() calling LoadGame() the StartingDoorLabel
	// and StartingLevelName will be set to values from the save
	// game. Then, when the AdventurePlayerController calls OnLoadRoom
	// with GameNotStarted, the save game room will be loaded.
	// Its important at this point that the Starting Level does _not_
	// have Permanently Loaded set to true in the levels window.

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
	
	AAdventurePlayerController *GetAdventureController() const;

	static UAdventureGameHUD *GetHUD();
	
	ADoor *FindDoor(FName DoorLabel);

	void LoadDoor(const ADoor *Door);

	FDelegateHandle OnInventoryChangedHandle;

	void LogSaveGameStatus(USaveGame *SaveGame);
};
