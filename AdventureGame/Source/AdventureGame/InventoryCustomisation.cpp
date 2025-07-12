
#include "InventoryCustomisation.h"

#include "AdventureGame.h"
#include "DetailCategoryBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "AdventurePlayerController.h"
#include "ItemList.h"

// https://kantandev.com/articles/details-panel-customization

#define LOCTEXT_NAMESPACE "InventoryCustomisation"

TSharedRef<ITableRow> FInventoryCustomisation::GenerateItemRow(
    TSharedPtr<FText> Item,
    const TSharedRef<STableViewBase>& OwnerTable)
{
    return SNew(STableRow<TSharedPtr<FText>>, OwnerTable)
        [
            SNew(STextBlock)
            .Text(*Item)
        ];
}

UItemList* FInventoryCustomisation::GetItemList()
{
    if (auto LocalInventory = this->Inventory.Get())
    {
        return LocalInventory;
    }
    if (const auto DetailBuilder = InventoryBuilder.Pin())
    {
        TArray<TWeakObjectPtr<UObject>> Objects;
        DetailBuilder->GetObjectsBeingCustomized(Objects);
        if (Objects.IsEmpty()) return nullptr;
        if (AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(Objects[0].Get()))
        {
            return AdventurePlayerController->GetInventoryItemList();
        }
    }
    else
    {
        UE_LOG(LogConfig, Warning, TEXT("Could not access DetailBuilder in FInventoryCustomisation"));
    }
    return nullptr;
}

TSharedRef<IDetailCustomization> FInventoryCustomisation::MakeInstance()
{
    return MakeShareable(new FInventoryCustomisation);
}

void FInventoryCustomisation::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder )
{
    InventoryBuilder = DetailBuilder.ToWeakPtr();
    CustomizeDetails(*DetailBuilder);
}

void FInventoryCustomisation::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    TArray<UInventoryItem*> InventoryItems;
    if (UItemList *Inventory = GetItemList())
    {
        Inventory->GetInventoryItemsArray(InventoryItems);
        if (!RefreshHandlerRegistered)
        {
            InventoryUpdateDelegateHandle = Inventory->OnInventoryChanged.AddRaw(this, &FInventoryCustomisation::HandleInventoryUpdate);
            RefreshHandlerRegistered = true;
        }
    }
    InventoryDescriptions.Empty();
    for (auto Element : InventoryItems)
    {
        const auto Desc = MakeShared<FText>(Element->GetDescription());
        InventoryDescriptions.Add(Desc);
    }
    IDetailCategoryBuilder& EngineCategory = DetailBuilder.EditCategory(("Inventory"));
    const FText PlayerInventory = LOCTEXT("PlayerInventoryTitle", "Player Inventory");
    // SListView - https://forums.unrealengine.com/t/tutorial-slate-slistview-example/46308
    // https://blog.robertlewicki.games/posts/list-views-in-slate/
    auto Row = EngineCategory.AddCustomRow(PlayerInventory)
        .NameContent()
            [
                SNew(STextBlock).Text(PlayerInventory)
            ]
        .ValueContent()
            .VAlign(VAlign_Center)
            .MaxDesiredWidth(300)
            [
                SNew(SListView<TSharedPtr<FText>>)
                .ItemHeight(24)
                .ListItemsSource(&InventoryDescriptions)
                .OnGenerateRow(this, &FInventoryCustomisation::GenerateItemRow)
            ];
}

void FInventoryCustomisation::HandleInventoryUpdate(FName InventoryName)
{
    if (const auto DetailLayoutBuilder = InventoryBuilder.Pin())
    {
        DetailLayoutBuilder->ForceRefreshDetails();
    }
}

#undef LOCTEXT_NAMESPACE
