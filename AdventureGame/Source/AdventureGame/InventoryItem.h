// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Enums/ItemKind.h"
#include "VerbInteractions.h"
#include "Internationalization/StringTableRegistry.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "InventoryItem.generated.h"

class AAdventurePlayerController;
class UItemList;

/**
 * The `Item` in our inventory.
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew)
class ADVENTUREGAME_API UInventoryItem : public UObject, public IVerbInteractions
{
    GENERATED_BODY()
public:
    //////////////////////////////////
    ///
    /// ITEM HANDLING
    ///

    /// Longer description used in "Look at" to make the item more real.
    /// eg "Blunt red knife with blood on it" - "Old tattered book covered with runes".
    /// By default uses the `DefaultItemDescriptionText` value in the `ItemStrings` string table.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    FText Description = LOCTABLE("ItemStrings", "DefaultItemDescriptionText");

    /// Very short description to distinguish this thing from others of the same kind
    /// eg "Blunt red knife" - "Old tattered book". By default uses the
    /// `DefaultItemDescriptionText` value in the `ItemStrings` string table.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    FText ShortDescription = LOCTABLE("ItemStrings", "DefaultItemDescriptionText");

    /// Enum ItemKind for this thing. Defaults to `None`. Add new
    /// item kinds in the `EItemKind` enum.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    EItemKind ItemKind = EItemKind::None;

    /// Thumbnail image to represent this item inside the inventory.
    /// Images used for the item while in the level or scene should
    /// be attached to a hotspot pickup instead.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    TObjectPtr<UPaperSprite> Thumbnail;

    /// An item kind that can meaningfully interact with this one. Used when
    /// items are combined in the inventory.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    EItemKind InteractableItem = EItemKind::None;

    /// Convenience method that combines this item with its interactable
    /// item to create a new item.
    ///
    /// Call this from the `OnItemCombineSuccess` event where the
    /// current item on the adventure controller is guaranteed to be
    /// correctly set to this items interactable item.
    ///
    /// To set the ShortDescription and Description for the item, create
    /// a blueprint and enum.
    /// 
    /// @param ResultingItem Kind of item to create.
    /// @param BarkText Text for player to say on creation.
    UFUNCTION(BlueprintCallable, Category = "ItemHandling")
    void CombineWithInteractableItem(EItemKind ResultingItem, FText BarkText = FText::GetEmpty());

    //////////////////////////////////
    ///
    /// EVENT HANDLERS
    ///

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemCombineSuccess();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemCombineFailure();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemGiveSuccess();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemGiveFailure();

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

    /// Player barks a message and then immediately ends any action sequence
    /// they were doing. Use when the blueprint event logic should end with a bark.
    /// @param BarkText FText for the player to bark. Should be translatable.
    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    void BarkAndEnd(FText BarkText);

    // https://medium.com/@lemapp09/learning-unreal-managing-object-lifecycles-in-unreal-engine-fa79ba9bb51c
    // Unreal Engine uses a garbage collection system to manage memory for UObject-derived classes.
    // The engine automatically tracks references to UObjects and collects them when they are no
    // longer needed.
    // 
    // Always use the `UPROPERTY` macro for UObject references to ensure proper garbage collection.
    // This helps the engine keep track of references and prevents premature destruction.
    //
    // Note: I did try using a Pimpl idiom here but that does not work with classes that are
    // exposed to Blueprints, as far as I can tell. Because those Blueprint exposed classes
    // have to be UObjects and have the destructor automatically generated.

    /// Inventory item local reference to the Adventure Player Controller object.
    /// Might be null, check before using.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemHandling")
    TWeakObjectPtr<AAdventurePlayerController> AdventurePlayerController;

    /// Inventory item local reference to the Item List that this item is a member of.
    /// Might be null, check before using.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemHandling")
    TWeakObjectPtr<UItemList> ItemList;
};
