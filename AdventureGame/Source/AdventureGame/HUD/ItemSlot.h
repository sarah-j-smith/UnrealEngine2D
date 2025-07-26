// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "ItemSlot.generated.h"

class UInventoryItem;
class UImage;

DECLARE_DYNAMIC_DELEGATE(FItemSlotDelegate);

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	FItemSlotDelegate OnItemClick;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton *ItemButton;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage *ItemSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool HasItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryItem *InventoryItem;

	void AddItem(UInventoryItem* InventoryItem);

	void RemoveItem();

	UFUNCTION()
	void HandleOnClicked();

	UFUNCTION()
	void HandleOnHover();

	UFUNCTION()
	void HandleOnUnhover();
private:
	FSlateBrush SavedStyle;
	
	void SetButtonImageFromInventoryItem(const UInventoryItem* InventoryItem);
};
