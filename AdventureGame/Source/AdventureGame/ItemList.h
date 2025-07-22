// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/List.h"

#include "ItemList.generated.h"

enum class EItemKind : uint8;
class UInventoryItem;

// https://unreal-garden.com/tutorials/delegates-advanced/#choosing-a-delegate-type

/**
 * Manage a list of items, for example as in a players inventory.
 */
UCLASS(Blueprintable, BlueprintType)
class ADVENTUREGAME_API UItemList : public UObject
{
    GENERATED_BODY()

private:
    /*
     * Use a Linked List here because the TArray throws exceptions
     * when you remove data from the middle of it due to the way it
     * tries to preserve the order of items, shuffling them up to
     * cover removed items. The semantics of the Inventory are that it
     * is an ordered list, not very long, and we often want to remove
     * items in the middle which Linked lists are great at. The list
     * is also pretty good for traversal to find items meeting a
     * condition. Items are always added at the end. This is a little
     * expensive as we don't keep a tail pointer or backlinks. But its
     * still very fast even for dozens of items.
     */
    typedef TList<UInventoryItem*> FInventoryList;
    typedef TArray<UInventoryItem*> FInventoryArray;
    FInventoryList* Inventory = nullptr;

    /**
    * Low-level helper that adds the inventory item in the list, creating a
    * `FInventoryList` element to hold it.
     * @param InventoryItem UInventoryItem to add.
     */
    void AddItemToInventory(UInventoryItem* InventoryItem);

    /**
    * Low-level helper that deletes the element from memory and invalidates
    * any iterators. The @param Element will be an invalid pointer after this call.
     * @param Element FInventoryList Element to delete from the list and from memory.
     */
    void DeleteElementFromInventory(FInventoryList* Element);

    /// Debugging tool.
    void DumpInventoryToLog() const;

public:
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryChangedSignature, FName /* Identifier */);
    FOnInventoryChangedSignature OnInventoryChanged;
    
    //////////////////////////////////
    ///
    /// CONFIGURATION
    ///

    /// In the case of multiple item lists this identifier distinguishes which one is updated.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Configuration")
    FName Identifier = "Inventory";

    /// Table of class references for creating instances of items.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* InventoryDataTable;

    //////////////////////////////////
    ///
    /// ITEM REPORTING
    ///

    /// A string describing a list of items. Mostly for debug purposes.
    static FString GetListDescription(const TArray<EItemKind>& List);

    /// Text for a generic item of that kind, used as a fallback to show to
    /// players, i18n via strings file. Generally overridden in a blueprint.
    static FText GetDescription(const EItemKind& ItemKind);

    /// Name that is unique among all items, not translated/shown to player,
    /// used to locate the item in the data tables.
    static FName GetUniqueName(const EItemKind& ItemKind);

    UFUNCTION()
    bool Contains(EItemKind Item) const;

    UFUNCTION()
    bool IsEmpty() const;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    int InventorySize = 0;

    //////////////////////////////////
    ///
    /// ITEM MANAGEMENT
    ///

    /**
    * Add the given item to the current players inventory of held
    * items. Instantiates an `UInventoryItem` instance of the class
    * from the `InventoryDataTable`. Caller should populate other fields
    * including `AdventureController`. If the Description field is empty
    * it defaults to using the enum's descriptive string.
    *
    * At present only one instance of any `ItemKind` can exist in the inventory
    * at a time. This might change in the future, but for now its not supported
    * to have more than one of anything.
    * 
    * @param ItemToAdd EItemKind to create an InventoryItem instance of.
    * @param Description FText optional description to give to the item
    * @return InventoryItem Created and added.
    */
    UFUNCTION(BlueprintCallable)
    UInventoryItem* AddItemToInventory(EItemKind ItemToAdd);

    /// Removes the given item from the current players inventory of held
    /// items. Destroys the `UInventoryItem` instance of the class
    /// from the `InventoryDataTable` and deletes it in the inventory UI
    UFUNCTION(BlueprintCallable)
    void RemoveItemKindFromInventory(EItemKind ItemToRemove);

    /// Removes the given items from the current players inventory of held
    /// items. Destroys the `UInventoryItem` instances of the class
    /// from the `InventoryDataTable` and deletes them in the inventory UI
    UFUNCTION(BlueprintCallable)
    void RemoveItemKindsFromInventory(const TSet<EItemKind>& ItemsToRemove);

    /// Copy pointers to the current inventory into the given array out argument.
    /// @param Result Reference to an array of Inventory Item pointers. This array will be emptied and over-written.
    void GetInventoryItemsArray(TArray<UInventoryItem *> &Result) const;
};
