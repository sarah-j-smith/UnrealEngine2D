#pragma once

#include "CoreMinimal.h"

#include "PromptData.h"
#include "ConversationData.h"

class MockDataTable : public UDataTable
{
    std::vector<std::string> ConversationData;
};

class FConversationTestUtils
{
public:
    static FPromptData CreatePromptData(
        int32 Number, int32 SubNumber, bool Visible, bool HasBeenSelected,
        bool SingleUse, const TCHAR* PromptText[], uint PromptTextCount,
        const TCHAR* NPCResponse[], uint NPCResponseCount,
        MockDataTable* SwitchDataTable, bool EndsConversation, const TCHAR* Event
    );

    static FConversationData CreateData();

    static FConversationData CreateEmptyTextRowData();

    static FConversationData CreateEmptyTextItemData();

    static FConversationData CreateBadSubSequenceData();

    static FConversationData CreateBadOrderSequenceData();
    
    static FConversationData CreateSingleUseErrorData();
    
    static FConversationData CreateLongBadSequenceData();
};
