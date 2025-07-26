// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventureGame/Enums/ItemKind.h"

#include "GameFramework/SaveGame.h"

#include "AdventureSave.generated.h"

class UAdventureGameInstance;
/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UAdventureSave : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Starting Level")
    FName StartingDoorLabel;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Starting Level")
    FName StartingLevel;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Items")
    TArray<EItemKind> Inventory;

    /// Called after known core game values are saved into this object by
    /// a call to <code>SaveGame</code> on the Game Instance. 
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save Game")
    void OnAdventureSave(const UAdventureGameInstance *GameInstance);

    /// Called after known core game values are loaded from this object by
    /// a call to <code>LoadGame</code> on the Game Instance. 
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save Game")
    void OnAdventureLoad(const UAdventureGameInstance *GameInstance);
};
