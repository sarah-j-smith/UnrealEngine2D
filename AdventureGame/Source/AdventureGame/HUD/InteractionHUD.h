// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "InteractionHUD.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UInteractionHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock *InteractionDescription;

	UFUNCTION(BlueprintCallable)
	void SetText(FText NewText) const;

	UFUNCTION(BlueprintCallable)
	void HighlightText();

	UFUNCTION(BlueprintCallable)
	void UnhighlightText();

	UFUNCTION(Blueprintable)
	void ResetText();

	/** Color when highlighted - eg D79115FF */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	FLinearColor TextHighlightColor = FLinearColor::Green;

	/** Color when normal - eg 7D652AFF */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	FLinearColor TextNormalColor = FLinearColor::Red;

private:
	bool TextLocked;
	
};
