// (c) 2025 Sarah Smith


#include "PromptList.h"

void UPromptList::NativeOnInitialized()
{
    PromptEntries.Append({ Prompt1, Prompt2, Prompt3, Prompt4, Prompt5 });

    Prompt1->PromptButton->OnClicked.AddUniqueDynamic(this, &UPromptList::OnPrompt1Clicked);
}

void UPromptList::HidePromptEntries()
{
    for (UDialogPrompt *Prompt : PromptEntries)
    {
        Prompt->HidePrompt();
    }
}


void UPromptList::SetPromptMenusEnabled(bool Enabled)
{
    for (UDialogPrompt *Prompt : PromptEntries)
    {
        Prompt->SetIsEnabled(Enabled);
    }
}

void UPromptList::SetPromptText(FText TextToBark, bool HasBeenSelected, uint8 RowIndex)
{
    PromptEntries[RowIndex]->HasBeenUsed = HasBeenSelected;
    PromptEntries[RowIndex]->ResetText();
    PromptEntries[RowIndex]->SetText(TextToBark);
}

void UPromptList::OnPrompt1Clicked()
{
    PromptClickedEvent.Broadcast(1);
}

void UPromptList::OnPrompt2Clicked()
{
    PromptClickedEvent.Broadcast(2);
}

void UPromptList::OnPrompt3Clicked()
{
    PromptClickedEvent.Broadcast(3);
}

void UPromptList::OnPrompt4Clicked()
{
    PromptClickedEvent.Broadcast(4);
}

void UPromptList::OnPrompt5Clicked()
{
    PromptClickedEvent.Broadcast(5);
}
