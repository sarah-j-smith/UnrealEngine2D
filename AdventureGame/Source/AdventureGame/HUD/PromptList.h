// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "DialogPrompt.h"
#include "Blueprint/UserWidget.h"
#include "PromptList.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UPromptList : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeOnInitialized() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UDialogPrompt *Prompt1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UDialogPrompt *Prompt2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UDialogPrompt *Prompt3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UDialogPrompt *Prompt4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UDialogPrompt *Prompt5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TArray<UDialogPrompt*> PromptEntries;

    UFUNCTION(BlueprintCallable)
    void HidePromptEntries();

    UFUNCTION(BlueprintCallable)
    void SetPromptText(FText TextToBark, bool HasBeenSelected, uint8 RowIndex);
};
