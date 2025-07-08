// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "VerbInteractions.h"
#include "ItemList.h"
#include "Internationalization/StringTableRegistry.h"
#include "Kismet/GameplayStatics.h"
#include "Paper2D/Classes/PaperSprite.h"

#include "InventoryItem.generated.h"

class AAdventurePlayerController;

/**
 * The `Item` in our inventory.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class ADVENTUREGAME_API UInventoryItem : public UObject, public IVerbInteractions
{
    GENERATED_UCLASS_BODY()
public:
    //////////////////////////////////
    ///
    /// ITEM HANDLING
    ///

    /// Very short description to distinguish this thing from others of the same kind
    /// eg "Blunt red knife" - "Old tattered book". By default uses the
    /// `DefaultItemDescriptionText` value in the `ItemStrings` string table.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    FText Description = LOCTABLE("ItemStrings", "DefaultItemDescriptionText");

    /// Enum ItemKind for this thing. Defaults to `None`. Add new
    /// item kinds in the `EItemList` enum.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    EItemList ItemKind = EItemList::None;

    /// Thumbnail image to represent this item inside the inventory.
    /// Images used for the item while in the level or scene should
    /// be attached to a hotspot pickup instead.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    TObjectPtr<UPaperSprite> Thumbnail;

    /// An item kind that can meaningfully interact with this one. Used when
    /// items are combined in the inventory.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    EItemList InteractableItem = EItemList::None;

    /// Convenience method that combines this item with its interactable
    /// item, which is assumed to be either the current item on the
    /// adventure controller.
    UFUNCTION(BlueprintCallable, Category = "ItemHandling")
    void CombineWithInteractableItem(EItemList ResultingItem, FText BarkText = FText::GetEmpty(),
                                     FText Desc = FText::GetEmpty());

    //////////////////////////////////
    ///
    /// EVENT HANDLERS
    ///

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemCombineSuccess();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemCombineFailure();

    //////////////////////////////////
    ///
    /// VERB INTERACTION
    ///

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnClose_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnOpen_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnGive_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnPickUp_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnTalkTo_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnLookAt_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnPull_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnPush_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnUse_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnWalkTo_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnItemUsed_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    virtual void OnItemGiven_Implementation() override;
    
    //////////////////////////////////
    ///
    /// ITEM TEXT
    ///

    static FText GetDescription(const UInventoryItem *Item);
    
    /// Get a string that describes this item. If there's a `Description`
    /// then simply return that. Otherwise get a descriptive string from
    /// the `UItemList` if available. All else fails use the
    /// DefaultItemDescriptionText as a default string.
    UFUNCTION(BlueprintCallable, Category = "VerbInteractions")
    FText GetDescription() const;

    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void BarkAndEnd(const FText& BarkText);

    /// Convenience method to get a (possibly null) reference to the
    /// AdventurePlayerController from GameplayStatics, for use by blueprints.
    /// Local code should dereference and use the local weak pointer.
    /// @return Possibly null AdventurePlayerController object.
    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    AAdventurePlayerController *GetAdventureController() const;

    /// Store a weak pointer to the AdventurePlayerController locally.
    void SetAdventureController(const TSharedRef<AAdventurePlayerController>& AdventurePlayerController)
    {
        this->AdventurePlayerController = TWeakPtr<AAdventurePlayerController>(AdventurePlayerController);
    }

    /// Convenience function to get the Inventory that this item is part of.
    /// @return Possibly null Item List object.
    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    UItemList *GetInventory() const
    {
        return this->ItemList.Get(false);
    }
    
    void SetItemList(UItemList *ItemList)
    {
        this->ItemList = ItemList;
    }

private:
    // https://prosser.io/optimizing-tweakobjectptr-usage/
    // https://dev.epicgames.com/community/learning/tutorials/kx/unreal-engine-all-about-soft-and-weak-pointers
    
    TWeakPtr<AAdventurePlayerController> AdventurePlayerController;

    TWeakObjectPtr<UItemList> ItemList;
};
