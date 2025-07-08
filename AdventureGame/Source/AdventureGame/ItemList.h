// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Containers/List.h"

#include "ItemList.generated.h"

class UInventoryItem;

UENUM(BlueprintType)
enum class EItemList: uint8
{
    None = 0 UMETA(DisplayName = "NONE"),
    Pickle = 1 UMETA(DisplayName = "PICKLE"),
    PickleKey = 2 UMETA(DisplayName = "PICKLE KEY"),
    Knife = 3 UMETA(DisplayName = "KNIFE"),
};

/**
 * 
 */
UCLASS()
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
    uint InventorySize = 0;


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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UDataTable* InventoryDataTable;

    /// A string describing a list of items.
    static FString GetListDescription(const TArray<EItemList>& List);

    /// Text for a generic item of that kind, used as a fallback to show to
    /// players, i18n via strings file. Generally overridden in a blueprint.
    static FText GetDescription(const EItemList& ItemKind);

    /// Name that is unique among all items, not shown to player, used to locate
    /// the item in the data tables.
    static FName GetUniqueName(const EItemList& ItemKind);

    UFUNCTION()
    bool Contains(EItemList Item) const;

    UFUNCTION()
    bool IsEmpty() const;

    /// Add the given item to the current players inventory of held
    /// items. Instantiates an `UInventoryItem` instance of the class
    /// from the `InventoryDataTable`. Caller should populate other fields
    /// including `AdventureController`. If the Description field is empty
    /// it defaults to using the enum's descriptive string.
    /// @param ItemToAdd EItemList to create an InventoryItem instance of.
    /// @param Description FText optional description to give to the item
    /// @return InventoryItem Created and added.
    UFUNCTION(BlueprintCallable)
    UInventoryItem* AddItemToInventory(EItemList ItemToAdd, FText Description = FText::GetEmpty());

    /// Removes the given item from the current players inventory of held
    /// items. Destroys the `UInventoryItem` instance of the class
    /// from the `InventoryDataTable` and deletes it in the inventory UI
    UFUNCTION(BlueprintCallable)
    void RemoveItemFromInventory(EItemList ItemToRemove);

    /// Removes the given items from the current players inventory of held
    /// items. Destroys the `UInventoryItem` instances of the class
    /// from the `InventoryDataTable` and deletes them in the inventory UI
    UFUNCTION(BlueprintCallable)
    void RemoveItemListFromInventory(const TSet<EItemList>& ItemsToRemove);

    UFUNCTION(BlueprintGetter)
    TArray<UInventoryItem*> GetInventoryItemsArray() const;

    /// Copy of the inventory Items exposed as a TArray as Blueprints will not
    /// work with a List.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter="GetInventoryItemsArray")
    TArray<UInventoryItem*> Items;
};
