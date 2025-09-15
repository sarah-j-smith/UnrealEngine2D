#include "ConversationData.h"

#include "AdventureGame/AdventureGame.h"
#include "AdventureGame/Constants.h"
#include "Algo/Count.h"

void FConversationData::DisplayPrompts(TArray<FPromptData>& OutPromptData) const
{
    OutPromptData.Empty();
    const int MaxPromptIndex = GetMaxPromptIndex();
    int PromptIndex = 0;
    int PromptSubindex = 0;
    int PromptCount = 0;
    while (PromptCount < GMax_Number_Of_Prompts && PromptIndex <= MaxPromptIndex)
    {
        if (const FPromptData* Prompt = FindPromptAtIndex(PromptIndex, PromptSubindex))
        {
            while (Prompt && Prompt->SingleUse && Prompt->HasBeenSelected)
            {
                Prompt = FindPromptAtIndex(PromptIndex, ++PromptSubindex);
            }
            if (Prompt)
            {
                OutPromptData.Add(*Prompt);
                ++PromptCount;
            }
        }
        PromptIndex++;
        PromptSubindex = 0;
    }
    if (OutPromptData.Num() == 0)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("No prompts selected."));
    }
}

void FConversationData::MarkPromptSelected(int PromptIndex, int SubIndex)
{
    FPromptData* FoundResult = ConversationPromptArray.FindByPredicate(
        [PromptIndex, SubIndex](const FPromptData& PromptData)
        {
            return PromptData.IsIndex(PromptIndex, SubIndex);
        });
    FoundResult->HasBeenSelected = true;
    if (FoundResult->SingleUse)
    {
        FoundResult->Visible = false;
    }
}

int FConversationData::PromptsAvailableCount() const
{
    int Count = 0;
    TSet<int32> PromptIndexes;
    for (FPromptData Prompt : ConversationPromptArray)
    {
        if (Prompt.CanBeShown() && !PromptIndexes.Contains(Prompt.PromptNumber))
        {
            PromptIndexes.Add(Prompt.PromptNumber);
            ++Count;
        }
    }
    return Count;
}

bool FConversationData::Validate(FString &ErrorMessage)
{
    int32 PreviousPromptIndex = -1;
    int32 PreviousPromptSubindex = -1;
    int row = 0;
    for (const FPromptData& Prompt : ConversationPromptArray)
    {
        if (Prompt.PromptText.IsEmpty() || Prompt.HasEmptyText())
        {
            ErrorMessage = FString::Printf(TEXT("Prompt %d (row %d) has empty text"), row, Prompt.PromptNumber);
            return false;
        }
        if (Prompt.EndsConversation && Prompt.SwitchTopic)
        {
            ErrorMessage = FString::Printf(TEXT("Prompt %d (row %d) - %s switches topic but ends conversation"),
                Prompt.PromptNumber, row, *(Prompt.PromptText[0].ToString()));
            return false;
        }
        if (!FPromptData::IsValidNextIndex(PreviousPromptIndex, PreviousPromptSubindex,
            Prompt.PromptNumber, Prompt.PromptSubNumber))
        {
            ErrorMessage = FString::Printf(
                TEXT("Prompt (row %d) Invalid sequence  Prev [ %d / %d ], Next [ Prompt: %d / Sub-prompt: %d ]"),
                row, PreviousPromptIndex, PreviousPromptSubindex, Prompt.PromptNumber, Prompt.PromptSubNumber);
            return false;
        }
        PreviousPromptIndex = Prompt.PromptNumber;
        PreviousPromptSubindex = Prompt.PromptSubNumber;
        row += 1;
    }
    return true;
}

const FPromptData* FConversationData::FindPromptAtIndex(int32 PromptIndex, int32 SubIndex) const
{
    return ConversationPromptArray.FindByPredicate(
        [PromptIndex, SubIndex](const FPromptData& PromptData)
        {
            return PromptData.PromptNumber == PromptIndex && PromptData.PromptSubNumber == SubIndex;
        });
}

int FConversationData::GetMaxPromptIndex() const
{
    int MaxPromptIndex = -1;
    for (FPromptData PromptData : ConversationPromptArray)
    {
        MaxPromptIndex = std::max(PromptData.PromptNumber, MaxPromptIndex);
    }
    return MaxPromptIndex;
}
