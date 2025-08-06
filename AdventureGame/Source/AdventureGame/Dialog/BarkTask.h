// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintAsyncActionBase.h"
#include "BarkTask.generated.h"

class USphereComponent;
class AAdventurePlayerController;

/// https://unrealcommunity.wiki/creating-asynchronous-blueprint-nodes-ctnmtj0q

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBarkOutputPin);

/**
 * Manage player asynchronous bark from blueprint actions.
 */
UCLASS()
class ADVENTUREGAME_API UBarkTask : public UBlueprintAsyncActionBase
{
    GENERATED_UCLASS_BODY()
public:
    UPROPERTY(BlueprintAssignable)
    FBarkOutputPin TaskCompleted;

    UPROPERTY(BlueprintAssignable)
    FBarkOutputPin TaskInterrupted;

    /**
     * Async task for the player-character to bark some text and then pause
     * for a time so the player can read it.
     * @param WorldContextObject Supplied by the blueprint, ignore.
     * @param BarkText Text to bark. Enter "\n" line feeds to split into rows.
     * @param BarkTime Amount of time for the player character to delay. Defaults to "0" which means use the time based on the length of the string.
     * @param BarkLocation Pointer to a USphereComponent that the text should try to follow. Defaults to NULL which means use the players bark location.
     * @param TextColor Color to bark. Defaults to white.
     * @return The created bark task will either end successfully or be interrupted. Use the appropriate pins to respond to each.
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
        Category = "Flow Control")
    static UBarkTask* DoBarkTask(const UObject* WorldContextObject, const FText BarkText,
                                 float BarkTime = 0,
                                 USphereComponent* BarkLocation = nullptr, FColor TextColor = FColor::White);

    virtual void Activate() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Task Control")
    const UObject* WorldContextObject;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Task Control")
    int32 MyUID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Barking")
    FText BarkText;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Barking")
    float TimeToBark;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Barking")
    USphereComponent* BarkLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Barking")
    FColor TextColor;

private:
    UFUNCTION()
    void BarkCompleted(FText Text, int32 UID, bool bSuccess);

    AAdventurePlayerController* GetAdventureController() const;
};
