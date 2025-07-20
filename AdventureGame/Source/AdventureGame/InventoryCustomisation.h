#pragma once

#include "AdventurePlayerController.h"
#include "IDetailCustomization.h"

class UInventoryItem;

#define LOCTEXT_NAMESPACE "InventoryCustomisation"

class FInventoryCustomisation :  public IDetailCustomization
{
public:

	///  TO DO - probably remove this - can be turned on in AdventureGame.cpp

	//////////////////////////////////
	///
	/// DETAILS CUSTOMIZATION TO SHOW INVENTORY
	///

	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	/// Version allowing storage of a weak pointer to the DetailBuilder
	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;
	
	void HandleInventoryUpdate(FName InventoryName);

	TSharedRef<ITableRow> GenerateItemRow(TSharedPtr<FText>, const TSharedRef<STableViewBase>& OwnerTable);

private:
	bool RefreshHandlerRegistered = false;

	TWeakPtr<IDetailLayoutBuilder> InventoryBuilder;

	TWeakObjectPtr<UItemList> Inventory;

	FDelegateHandle InventoryUpdateDelegateHandle;

	UItemList *GetItemList();

	TArray<TSharedPtr<FText>> InventoryDescriptions;

	TSharedPtr<SListView<TSharedPtr<FText>>> InventoryListView;
};

#undef LOCTEXT_NAMESPACE