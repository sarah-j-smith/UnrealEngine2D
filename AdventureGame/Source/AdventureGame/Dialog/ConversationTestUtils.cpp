#include "ConversationTestUtils.h"


#include "PromptData.h"
#include "Misc/GeneratedTypeName.h"

typedef const TCHAR* CharData[3][3];
 CharData PromptText = {
    {TEXT("Hello. My name is Inigo Montoya.")},
    {TEXT("You killed my father"), TEXT("Prepare to die")},
    {TEXT("I just cut off you hand"), TEXT("What do you say now?")}};

 CharData ResponseText = {
    {TEXT("I am Count Rugen.")},
    {TEXT("You are that sad little boy"), TEXT("Now I will have to kill you"), TEXT("How tragic")},
    {TEXT("Bye"), TEXT("Damn this door is stuck"), TEXT("Can I plead for mercy?")}};

FName SomeEvent = TEXT("Some event");

FPromptData FConversationTestUtils::CreatePromptData(
    int32 Number, int32 SubNumber, bool Visible, bool HasBeenSelected,
    bool SingleUse, const TCHAR* PromptText[], uint PromptTextCount,
    const TCHAR* NPCResponse[], uint NPCResponseCount,
    MockDataTable* SwitchDataTable = nullptr, bool EndsConversation = false, const TCHAR* Event = nullptr
)
{
    FPromptData PromptData;
    PromptData.PromptNumber = Number;
    PromptData.PromptSubNumber = SubNumber;
    PromptData.Visible = Visible;
    PromptData.HasBeenSelected = HasBeenSelected;
    PromptData.SingleUse = SingleUse;
    {
        TArray<FText> PromptTextArray;
        for (uint i = 0; i < PromptTextCount; i++)
        {
            PromptTextArray.Add(FText::FromString(PromptText[i]));
        }
        PromptData.PromptText = PromptTextArray;
    }
    {
        TArray<FText> NPCResponseTextArray;
        for (uint i = 0; i < NPCResponseCount; i++)
        {
            NPCResponseTextArray.Add(FText::FromString(NPCResponse[i]));
        }
        PromptData.NPCResponse = NPCResponseTextArray;
    }
    PromptData.SwitchTopic = SwitchDataTable;
    PromptData.EndsConversation = EndsConversation;
    PromptData.Event = Event;
    return PromptData;
}

FConversationData CreateSequenceData(std::vector<int> PromptIndexes, std::vector<int> PromptSubIndexes)
{
    FConversationData ConversationData;

    for (uint i = 0; i < PromptIndexes.size(); i++)
    {
        int StringsIndex = i % 3;
        ConversationData.ConversationPromptArray.Add(
            FConversationTestUtils::CreatePromptData(PromptIndexes[i], PromptSubIndexes[i],
                true, false,
                                                     false, PromptText[StringsIndex], 1,
                                                     ResponseText[StringsIndex], 1,
                                                     nullptr, false, TEXT("Some event")
            ));
    }
    return ConversationData;
}

FConversationData FConversationTestUtils::CreateData()
{
    return CreateSequenceData({ 0, 1, 1 }, { 0, 0, 1 });
}

FConversationData FConversationTestUtils::CreateEmptyTextRowData()
{
    FConversationData ConversationData;

    const TCHAR* EmptyText2[] = {}; // empty array

    ConversationData.ConversationPromptArray.Add(
        FConversationTestUtils::CreatePromptData(0, 0, true, false,
                                                 false, PromptText[0], 1,
                                                 ResponseText[0], 1,
                                                 nullptr, false, TEXT("Some event")
        ));
    ConversationData.ConversationPromptArray.Add(
        FConversationTestUtils::CreatePromptData(1, 0, true, false,
                                                 true, EmptyText2, 0,
                                                 ResponseText[1], 2,
                                                 nullptr, false, TEXT("")
        )
    );
    return ConversationData;
}

FConversationData FConversationTestUtils::CreateEmptyTextItemData()
{
    FConversationData ConversationData;

    const TCHAR* EmptyItem2[] = {TEXT("You killed my father"), TEXT("")}; // empty element

    ConversationData.ConversationPromptArray.Add(
        FConversationTestUtils::CreatePromptData(0, 0, true, false,
                                                 false, PromptText[0], 1,
                                                 ResponseText[0], 1,
                                                 nullptr, false, TEXT("Some event")
        ));
    ConversationData.ConversationPromptArray.Add(
        FConversationTestUtils::CreatePromptData(1, 0, true, false,
                                                 true, EmptyItem2, 2,
                                                 ResponseText[1], 2,
                                                 nullptr, false, TEXT("")
        )
    );
    return ConversationData;
}

FConversationData FConversationTestUtils::CreateBadOrderSequenceData()
{
    // The prompt indexes are not in order, the last 1 is out of sequence
    return CreateSequenceData({ 0, 1, 1, 2, 1 },{ 0, 0, 1, 0, 0 });
}

FConversationData FConversationTestUtils::CreateBadSubSequenceData()
{
    // The PromptSubIndexes for this should be { 0, 0, 1, 0, 0 }
    return CreateSequenceData({ 0, 1, 1, 2, 3 },{ 0, 0, 0, 0, 0 });
}

FConversationData FConversationTestUtils::CreateLongBadSequenceData()
{
    return CreateSequenceData({ 0, 1, 1, 2, 2, 2, 3, 4, 5, 6, 6, 7 },
        { 0, 0, 1, 0, 1, /* should be 2*/ 0, 0, 0, 0, 0, 1, 0 });
}
