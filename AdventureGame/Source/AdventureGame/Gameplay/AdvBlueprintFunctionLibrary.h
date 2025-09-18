// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Enums/DoorState.h"
#include "AdvBlueprintFunctionLibrary.generated.h"

class UAdventureGameInstance;
enum class EItemKind: uint8;
class UInventoryItem;
class AAdventurePlayerController;

#define SHORT_LETTER_COUNT 12
#define MEDIUM_LETTER_COUNT 20
#define LONG_LETTER_COUNT 30
#define EXTRA_LONG_LETTER_COUNT 45

#define SHORT_BARK_TIME 1.5f
#define MEDIUM_BARK_TIME 3.0f
#define LONG_BARK_TIME 5.0f
#define EXTRA_LONG_BARK_TIME 8.0f

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UAdvBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Player Actions",
        meta = (WorldContext = "WorldContextObject"))
    static AAdventurePlayerController* GetAdventureController(const UObject* WorldContextObject);

    UFUNCTION(BlueprintCallable, Category = "Player Actions", meta = (WorldContext = "WorldContextObject"))
    static void PlayerBark(const UObject* WorldContextObject, FText BarkText);

    UFUNCTION(BlueprintCallable, Category = "Player Actions", meta = (WorldContext = "WorldContextObject"))
    static void ClearVerb(const UObject* WorldContextObject);

    /// Add a new item to the inventory and return a reference to it if successful.
    /// @param ItemToAdd The kind of item to add. The text descriptions for each kind can be added in the text resource.
    UFUNCTION(BlueprintCallable, Category = "Player Actions", meta = (WorldContext = "WorldContextObject"))
    static UInventoryItem* AddToInventory(const UObject* WorldContextObject, EItemKind ItemToAdd);

    UFUNCTION(BlueprintCallable, Category = "Player Actions", meta = (WorldContext = "WorldContextObject"))
    static void RemoveFromInventory(const UObject* WorldContextObject, EItemKind ItemToRemove);

    UFUNCTION(BlueprintPure, Category = "Debug", meta = (WorldContext = "WorldContextObject"))
    static int32 PIEInstance(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure)
    static FString GetProjectVersion();

    UFUNCTION(BLueprintCallable, Category = "Player Actions", BlueprintPure)
    static float GetBarkTime(const FString &BarkText);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Save Game", meta = (WorldContext = "WorldContextObject"))
    static UAdventureGameInstance* GetAdventureInstance(const UObject* WorldContextObject);
    
    /**
     * Check based on state if the door or item can be locked
     * @param DoorState Current state of the door or item
     * @return true if the door or item can be locked
     */
    UFUNCTION(BLueprintCallable, Category = "Door State", BlueprintPure)
    bool CanLock(EDoorState DoorState) { return CanLockDoorOrItem(DoorState); }

    /**
     * Check if the door or item can be unlocked.
     * @param DoorState Current state of the door or openable item
     * @return true if the door or item can be unlocked
     */
    UFUNCTION(BLueprintCallable, Category = "Door State", BlueprintPure)
    bool CanUnlock(EDoorState DoorState) { return CanUnlockDoorOrItem(DoorState); };

    /**
     * Check based on state if the door or item can be locked
     * @param DoorState Current state of the door or item
     * @return true if the door or item can be locked
     */
    UFUNCTION(BLueprintCallable, Category = "Door State", BlueprintPure)
    bool CanOpen(EDoorState DoorState) { return CanOpenDoorOrItem(DoorState); };

    /**
     * Check based on state if the door or item can be locked
     * @param DoorState Current state of the door or item
     * @return true if the door or item can be locked
     */
    UFUNCTION(BLueprintCallable, Category = "Door State", BlueprintPure)
    bool CanClose(EDoorState DoorState) { return CanCloseDoorOrItem(DoorState); };
};
