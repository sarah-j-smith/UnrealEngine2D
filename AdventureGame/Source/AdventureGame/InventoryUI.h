// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "ItemSlot.h"
#include "Blueprint/UserWidget.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UInventoryUI : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeOnInitialized() override;
	
	//////////////////////////////////
	///
	/// ARROWS
	///
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Arrows")
	UButton *UpArrowButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Arrows")
	UButton *DownArrowButton;

	UFUNCTION(BlueprintCallable, Category = "Arrows")
	void OnDownArrowButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Arrows")
	void OnUpArrowButtonClicked();

	void CalculateMaxRowIndex();

	void UpdateCurrentRowIndex();

	void PopulateInventory();

	int32 MaxRowIndex = 0;
	int32 CurrentRowIndex = 0;

private:
	bool bArrowButtonClicked = false;
	int32 iCurrentInventoryLength = 0;

	void UpdateInventoryLength();
	
	//////////////////////////////////
	///
	/// INVENTORY SLOTS
	///

public:
	TArray<UItemSlot *> InventorySlots;

	void AddSlotsToArray();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *TopItem1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *TopItem2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *TopItem3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *TopItem4;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *BottomItem1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *BottomItem2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *BottomItem3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), Category = "Inventory Slots")
	UItemSlot *BottomItem4;
};
