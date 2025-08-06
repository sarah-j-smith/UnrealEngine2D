#include "ConversationData.h"

#include "AdventureGame/AdventureGame.h"
#include "AdventureGame/Constants.h"

TArray<FPromptData> FConversationData::DisplayPrompts()
{
    TArray<FPromptData> PromptsToDisplay;
    const int MaxPromptIndex = GetMaxPromptIndex();
    int PromptIndex = 0;
    int PromptSubindex = 0;
    int PromptCount = 0;
    while (PromptCount < GMax_Number_Of_Prompts && PromptIndex <= MaxPromptIndex)
    {
        if (const FPromptData *Prompt = FindPromptAtIndex(PromptIndex, PromptSubindex))
        {
            while (Prompt && Prompt->SingleUse && Prompt->HasBeenSelected)
            {
                Prompt = FindPromptAtIndex(PromptIndex, ++PromptSubindex);
            }
            if (Prompt)
            {
                PromptsToDisplay.Add(*Prompt);
                ++PromptCount;
            }
        }
        PromptIndex++;
        PromptSubindex = 0;
    }
    if (PromptsToDisplay.Num() == 0)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("No prompts selected."));
    }
    return PromptsToDisplay;
}

FPromptData * FConversationData::FindPromptAtIndex(int32 PromptIndex, int32 SubIndex)
{
    return ConversationPromptArray.FindByPredicate(
        [PromptIndex, SubIndex](const FPromptData& PromptData)
        {
            return PromptData.PromptNumber == PromptIndex && PromptData.PromptSubNumber == SubIndex;
        });
}

int FConversationData::GetMaxPromptIndex()
{
    int MaxPromptIndex = -1;
    for (FPromptData PromptData : ConversationPromptArray)
    {
        MaxPromptIndex = std::max(PromptData.PromptNumber, MaxPromptIndex);
    }
    return MaxPromptIndex;
}
