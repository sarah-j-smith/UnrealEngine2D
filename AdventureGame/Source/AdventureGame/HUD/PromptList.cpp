// (c) 2025 Sarah Smith


#include "PromptList.h"

#include "AdventureGame/AdventureGame.h"

void UPromptList::NativeOnInitialized()
{
    PromptEntries.Append({ Prompt1, Prompt2, Prompt3, Prompt4, Prompt5 });

    Prompt1->PromptButton->OnClicked.AddUniqueDynamic(this, &UPromptList::OnPrompt1Clicked);
    Prompt2->PromptButton->OnClicked.AddUniqueDynamic(this, &UPromptList::OnPrompt2Clicked);
    Prompt3->PromptButton->OnClicked.AddUniqueDynamic(this, &UPromptList::OnPrompt3Clicked);
    Prompt4->PromptButton->OnClicked.AddUniqueDynamic(this, &UPromptList::OnPrompt4Clicked);
    Prompt5->PromptButton->OnClicked.AddUniqueDynamic(this, &UPromptList::OnPrompt5Clicked);
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
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("SetPromptText: %d [ %hs ] - %s"), RowIndex, HasBeenSelected ? "selected" : "novel", *TextToBark.ToString());
    PromptEntries[RowIndex]->HasBeenUsed = HasBeenSelected;
    PromptEntries[RowIndex]->SetText(TextToBark);
    PromptEntries[RowIndex]->UnhighlightText();
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
