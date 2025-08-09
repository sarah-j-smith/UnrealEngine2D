// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventureGame/Enums/ItemAssetType.h"
#include "AdventureGame/Enums/ItemKind.h"
#include "ItemDataAsset.generated.h"

class AAdventurePlayerController;
class UItemList;
class UInventoryItem;
class AHotSpot;

/**
 * Data and functions to handle items interacting with hotspots or other
 * items, in a re-usable way. Placing the functionality here avoids
 * duplicating it in each item that needs to have the behaviour.
 */
UCLASS()
class ADVENTUREGAME_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/// How to treat the source item when using the default implementation
	/// of OnItemUseSuccess.
	/// Article - nothing, Consumable - destroy,
	/// Tool - create the ToolResultItem, Key - Unlock the target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	EItemAssetType SourceItemAssetType;

	/// The <b>first</b> item required for the action to successfully initiate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	EItemKind SourceItem;

	/// How to treat the target item when using the default implementation
	/// of OnItemUseSuccess.
	/// Article - nothing, Consumable - destroy,
	/// Tool - create the ToolResultItem, Key - Unlock the target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	EItemAssetType TargetItemAssetType;

	/// The <b>second item</b> required for the action to successfully initiate, or null.
	/// There must be either this set, or the <code>HotSpot</code> set.m8
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	EItemKind TargetItem;

	/// If the items are used the other way around, does it matter?
	/// Using a TV remote on a TV might matter - may want to say "false"
	/// since "Use TV on remote" does not sound right. But if its
	/// like mixing two ingredients "Use flour on water" should be the
	/// same as "Use water on flour" - so use <code>true</code> which
	/// is the default.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	bool CanSwapSourceAndTarget = true;

	/// What text to bark if the Use is successful
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BarkText")
	FText UseSuccessBarkText;

	/// What sound to play if the Use is successful
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	USoundWave *UseSuccessSound;

	/// What text to bark if the Use fails
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BarkText")
	FText UseFailureBarkText;

	/// What text to bark if the Give is successful
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BarkText")
	FText GiveSuccessBarkText;

	/// What text to bark if the Give fails
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BarkText")
	FText GiveFailureBarkText;

	/// If the tool is used on a consumable, what should the result be
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemAction")
	EItemKind ToolResultItem;

	/// How many seconds should the players command remain highlighted
	/// and input locked after executing it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionTimeout = 5.0f;

	UFUNCTION(BlueprintCallable, Category="ItemAction")
	void StartTimer();

	UFUNCTION(BlueprintCallable, Category="ItemAction")
	void StopTimer();

	/// Triggered when this item is the <b>target</b> of a use verb, and it successfully
	/// passes initial checks. The source item of the use verb is found by querying
	/// the adventure player controller's <code>SourceItem</code> property.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
	void OnItemUseSuccess();

	/// Triggered when this item is the <b>target</b> of a give verb, and it successfully
	/// passes initial checks. The source item of the give verb is found by querying
	/// the adventure player controller's <code>SourceItem</code> property.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
	void OnItemGiveSuccess();

	/// Triggered when this item is the <b>target</b> of a use verb, and it successfully
	/// passes initial checks. The source item of the use verb is found by querying
	/// the adventure player controller's <code>SourceItem</code> property.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
	void OnItemUseFailure();

	/// Triggered when this item is the <b>target</b> of a give verb, and it successfully
	/// passes initial checks. The source item of the give verb is found by querying
	/// the adventure player controller's <code>SourceItem</code> property.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
	void OnItemGiveFailure();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool TimerRunning = false;
	
	UFUNCTION()
	void OnInteractionTimeout();

	void SetAdventurePlayerController(AAdventurePlayerController *PlayerController);
	
	FTimerHandle ActionHighlightTimerHandle;
private:
	TWeakObjectPtr<AAdventurePlayerController> Apc;
};
