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
