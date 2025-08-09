// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "InteractionHUD.h"
#include "InventoryUI.h"
#include "PromptList.h"
#include "VerbsUI.h"
#include "AdventureGame/Dialog/BarkText.h"

#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "AdventureGameHUD.generated.h"

class USphereComponent;
class AAdventureCharacter;
class AAdventurePlayerController;

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UAdventureGameHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void NativeOnInitialized() override;

	/// Bindings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInteractionHUD *InteractionUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerbsUI *VerbsUI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInventoryUI *InventoryUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UWidgetSwitcher *UISwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UPromptList *PromptList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBarkText *Bark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage *BlackScreen;

	UFUNCTION()
	void HandleInventoryChanged(FName Identifier);

	UFUNCTION(BlueprintCallable)
	void UpdateInteractionTextEvent();

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryTextEvent();

	UFUNCTION(BlueprintCallable)
	void BeginActionEvent();

	UFUNCTION(BlueprintCallable)
	void InterruptActionEvent();
	
	void ShowBlackScreen();

	void HideBlackScreen();

	void SetInteractionText();

	void SetInventoryText();

	void ShowPromptList();

	void HidePromptList();

	void AddBarkText(const FText &BarkText, USphereComponent *Position,
		TOptional<FColor> TextColor = TOptional<FColor>());

	void ClearBarkText();
	
private:
    bool IsMobileTouch = false;
	
	TWeakObjectPtr<AAdventureCharacter> PlayerCharacter;

	TWeakObjectPtr<AAdventurePlayerController> AdventurePlayerController;
};
