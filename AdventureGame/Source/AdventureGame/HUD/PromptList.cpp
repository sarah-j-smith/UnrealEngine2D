// (c) 2025 Sarah Smith


#include "PromptList.h"

void UPromptList::NativeOnInitialized()
{
    PromptEntries.Append({ Prompt1, Prompt2, Prompt3, Prompt4, Prompt5 });
}

void UPromptList::HidePromptEntries()
{
    for (UDialogPrompt *Prompt : PromptEntries)
    {
        Prompt->HidePrompt();
    }
}

void UPromptList::SetPromptText(FText TextToBark, bool HasBeenSelected, uint8 RowIndex)
{
    PromptEntries[RowIndex]->HasBeenUsed = HasBeenSelected;
    PromptEntries[RowIndex]->ResetText();
    PromptEntries[RowIndex]->SetText(TextToBark);
}
