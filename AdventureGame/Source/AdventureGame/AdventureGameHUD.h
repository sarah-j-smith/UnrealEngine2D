// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "InteractionHUD.h"
#include "InventoryUI.h"
#include "VerbsUI.h"

#include "Blueprint/UserWidget.h"
#include "AdventureGameHUD.generated.h"

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

	TObjectPtr<AAdventureCharacter> PlayerCharacter;

	TObjectPtr<AAdventurePlayerController> AdventurePlayerController;

	void SetInteractionText();

	UFUNCTION(BlueprintCallable)
	void UpdateInteractionTextEvent();

	UFUNCTION(BlueprintCallable)
	void BeginActionEvent();

	UFUNCTION(BlueprintCallable)
	void InterruptActionEvent();
};
