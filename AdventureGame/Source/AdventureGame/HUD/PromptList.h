// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "DialogPrompt.h"
#include "Blueprint/UserWidget.h"

#include "PromptList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPromptClickedDelegate, int, PromptIndex);

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UPromptList : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeOnInitialized() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FOnPromptClickedDelegate PromptClickedEvent;

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

    void SetPromptMenusEnabled(bool Enabled);

    UFUNCTION(BlueprintCallable)
    void SetPromptText(FText TextToBark, bool HasBeenSelected, uint8 RowIndex);

private:
    UFUNCTION()
    void OnPrompt1Clicked();
    
    UFUNCTION()
    void OnPrompt2Clicked();
    
    UFUNCTION()
    void OnPrompt3Clicked();
    
    UFUNCTION()
    void OnPrompt4Clicked();
    
    UFUNCTION()
    void OnPrompt5Clicked();
};
