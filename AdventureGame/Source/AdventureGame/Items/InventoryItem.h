// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "../Enums/ItemKind.h"
#include "../Gameplay/VerbInteractions.h"
#include "AdventureGame/Enums/DoorState.h"
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

    /// Data Asset for determining results of using this item. Use Success and
    /// asset interrogation will only occur once a test against the <code>InteractableItem</code>
    /// succeeds.
    ///
    /// The Data Asset is an <b>instance</b> of the <code>ItemDataAsset</code> sub-class, not the
    /// class itself. Right-click in the content drawer, and choose <i>Miscellaneous > Data Asset</i>
    /// then choose one of the <code>ItemDataAsset</code> sub-classes to create an instance.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    TSoftObjectPtr<UItemDataAsset> OnUseSuccessItem;

    /// Data Asset for determining results of successfully giving this item. This should be
    /// an <b>instance</b> of the <code>ItemDataAsset</code> sub-class, not the
    /// class itself. Right-click in the content drawer, and choose <i>Miscellaneous > Data Asset</i>
    /// then choose one of the <code>ItemDataAsset</code> sub-classes to create an instance.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    TSoftObjectPtr<UItemDataAsset> OnGiveSuccessItem;

    /// An item kind that can meaningfully interact with this one. Used when
    /// items are combined in the inventory.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    EItemKind InteractableItem = EItemKind::None;

    //////////////////////////////////
    ///
    /// EVENT HANDLERS
    ///

    /// Triggered when this item is the <b>target</b> of a use verb, and it successfully
    /// passes initial checks. The source item of the use verb is found by querying
    /// the adventure player controller's <code>SourceItem</code> property.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemUseSuccess();

    /// Triggered when this item is the <b>target</b> of a use verb, and it fails to
    /// pass initial checks. The source item of the use verb is found by querying
    /// the adventure player controller's <code>SourceItem</code> property.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemUseFailure();

    /// Triggered when this item is the <b>target</b> of a give verb, and it successfully
    /// passes initial checks. The source item of the give verb is found by querying
    /// the adventure player controller's <code>SourceItem</code> property.
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ItemHandling")
    void OnItemGiveSuccess();

    /// Triggered when this item is the <b>target</b> of a give verb, and it fails to
    /// pass initial checks. The source item of the use verb is found by querying
    /// the adventure player controller's <code>SourceItem</code> property.
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
    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    AAdventurePlayerController *GetAdventurePlayerController() const;

    /// Inventory item local reference to the Item List that this item is a member of.
    /// Might be null, check before using.
    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    UItemList *GetItemList() const;

    void SetAdventurePlayerController(AAdventurePlayerController* Controller);

    void SetItemList(UItemList *ItemList);

    //////////////////////////////////
    ///
    /// ITEM DISPOSITION
    ///

    /// Is the item open or close, or locked?  For items where those concepts make
    /// no sense this should stay in the <code>Unknown</code> state.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemHandling")
    EDoorState DoorState = EDoorState::Unknown;

    /// If this Inventory Item can interact with <code>ItemToInteract</code> then
    /// return true, otherwise return false. 
    UFUNCTION(BlueprintCallable, Category = "Player Actions")
    bool CanInteractWith(const UInventoryItem* ItemToInteract) const
    {
        return ItemToInteract->InteractableItem == ItemKind || ItemToInteract->ItemKind == InteractableItem;
    }
    
private:

    TWeakObjectPtr<AAdventurePlayerController> AdventurePlayerController;
    
    TWeakObjectPtr<UItemList> ItemList;
};
