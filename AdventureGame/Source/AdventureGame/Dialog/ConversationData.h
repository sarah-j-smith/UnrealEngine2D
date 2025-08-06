#pragma once

#include "CoreMinimal.h"

#include "PromptData.h"

#include "ConversationData.generated.h"

USTRUCT(BlueprintType)
struct ADVENTUREGAME_API FConversationData: public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    TArray<FPromptData> ConversationPromptArray;

    TArray<FPromptData> DisplayPrompts();
private:
    FPromptData *FindPromptAtIndex(int32 PromptIndex, int32 SubIndex);
    int GetMaxPromptIndex();
    
};
