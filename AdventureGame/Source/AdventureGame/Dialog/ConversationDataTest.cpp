#include "HotSpotNPC.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ConversationDataTest, "DefaultNamespace.ConversationDataTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


class MockDataTable : public UDataTable
{
    std::vector<std::string> ConversationData;
};

FPromptData CreatePromptData(
    int32 Number, int32 SubNumber, bool Visible, bool HasBeenSelected,
    bool SingleUse, const TCHAR* PromptText[], uint PromptTextCount,
    const TCHAR* NPCResponse[], uint NPCResponseCount,
    MockDataTable* SwitchDataTable, bool EndsConversation, const TCHAR* Event
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
        for (uint i = 0; i < PromptTextCount; i++) PromptTextArray.Add(FText::FromString(PromptText[i]));
        PromptData.PromptText = PromptTextArray;
    }
    {
        TArray<FText> NPCResponseTextArray;
        for (uint i = 0; i < NPCResponseCount; i++) NPCResponseTextArray.Add(FText::FromString(NPCResponse[i]));
        PromptData.NPCResponse = NPCResponseTextArray;
    }
    PromptData.SwitchTopic = SwitchDataTable;
    PromptData.EndsConversation = EndsConversation;
    PromptData.Event = Event;
    return PromptData;
}

bool ConversationDataTest::RunTest(const FString& Parameters)
{
    // Make the test pass by returning true, or fail by returning false.
    FConversationData ConversationData;

    const TCHAR* PromptText1[] = { TEXT("Hello. My name is Inigo Montoya.") };
    const TCHAR* ResponseText1[] = { TEXT("I am Count Rugen.") };
    const TCHAR* PromptText2[] = { TEXT("You killed my father"), TEXT("Prepare to die")};
    const TCHAR* ResponseText2[] = { TEXT("You are that sad little boy"), TEXT("Now I will have to kill you"), TEXT("How tragic")};
    const TCHAR* PromptText3[] = { TEXT("I just cut off you hand"), TEXT("What do you say now?")};
    const TCHAR* ResponseText3[] = { TEXT("Bye"), TEXT("Damn this door is stuck"), TEXT("Can I plead for mercy?")};
    FName SomeEvent = TEXT("Some event");

    ConversationData.ConversationPromptArray.Add(
        CreatePromptData(0, 0, true, false,
                         false, PromptText1, 1,
                         ResponseText1, 1,
                         nullptr, false, TEXT("Some event")
        ));
    ConversationData.ConversationPromptArray.Add(
        CreatePromptData(1, 0, true, false,
                         true, PromptText2, 2,
                         ResponseText2, 2,
                         nullptr, false, TEXT("")
        )
    );
    ConversationData.ConversationPromptArray.Add(
        CreatePromptData(1, 1, true, false,
                         false, PromptText3, 2,
                         ResponseText3, 3,
                         nullptr, false, TEXT("")
        )
    );

    TestEqual(TEXT("Count is correct"), ConversationData.PromptsAvailableCount(), 2);

    TArray<FPromptData> PromptsToDisplay;
    ConversationData.DisplayPrompts(PromptsToDisplay);
    TestEqual(TEXT("Query members number"), PromptsToDisplay[0].PromptNumber, 0);
    TestEqual(TEXT("Query members text"), PromptsToDisplay[0].PromptNumber, 0);
    TestEqual(TEXT("Query members second number"), PromptsToDisplay[1].PromptNumber, 1);
    TestEqual(TEXT("Query members second txt"), PromptsToDisplay[1].PromptNumber, 1);

    ConversationData.MarkPromptSelected(0, 0);
    ConversationData.DisplayPrompts(PromptsToDisplay);
    TestTrue(TEXT("Query members selected"), PromptsToDisplay[0].HasBeenSelected);
    TestTrue(TEXT("Query members selected"), PromptsToDisplay[0].Visible);
    TestEqual(TEXT("Count is correct"), ConversationData.PromptsAvailableCount(), 2);
    
    TestEqual(TEXT("Query members text"), PromptsToDisplay[0].PromptNumber, 0);
    TestEqual(TEXT("Query members second number"), PromptsToDisplay[1].PromptNumber, 1);
    TestEqual(TEXT("Query members second txt"), PromptsToDisplay[1].PromptNumber, 1);


    ConversationData.MarkPromptSelected(1, 0);
    
    return true;
}
