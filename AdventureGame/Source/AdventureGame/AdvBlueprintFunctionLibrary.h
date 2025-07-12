// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "ItemKind.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AdvBlueprintFunctionLibrary.generated.h"

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

    UFUNCTION(BlueprintCallable, Category = "Player Actions", meta = (WorldContext = "WorldContextObject"))
    static UInventoryItem* AddToInventory(const UObject* WorldContextObject, EItemKind ItemToAdd,
                                          FText Description = FText::GetEmpty(),
                                          EItemKind InteractableItem = EItemKind::None);

    UFUNCTION(BlueprintCallable, Category = "Player Actions", meta = (WorldContext = "WorldContextObject"))
    static void RemoveFromInventory(const UObject* WorldContextObject, EItemKind ItemToRemove);

    UFUNCTION(BlueprintPure, Category = "Debug", meta = (WorldContext = "WorldContextObject"))
    static int32 PIEInstance(const UObject* WorldContextObject);

    UFUNCTION(BlueprintPure)
    static FString GetProjectVersion();
};
