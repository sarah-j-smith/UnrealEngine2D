// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AdvBlueprintFunctionLibrary.generated.h"

enum class EItemKind: uint8;
class UInventoryItem;
class AAdventurePlayerController;

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
};
