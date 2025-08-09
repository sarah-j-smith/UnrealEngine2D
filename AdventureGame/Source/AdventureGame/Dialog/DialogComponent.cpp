// (c) 2025 Sarah Smith


#include "DialogComponent.h"

#include "ConversationData.h"


// Sets default values for this component's properties
UDialogComponent::UDialogComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UDialogComponent::BeginPlay()
{
    Super::BeginPlay();
    
    ConversationDataLoad.ExecuteIfBound(this);
    FillConversationData();
}


// Called every frame
void UDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}


void UDialogComponent::FillConversationData()
{
    if (ConversationData.Num() > 0) return; // already filled
    for (const auto TopicTable : TopicList)
    {
        TArray<FPromptData> PromptsForTopic;
        TArray<FPromptData*> PromptPtrs;
        TopicTable->GetAllRows<FPromptData>("FillConversationData", PromptPtrs);
        for (const FPromptData *PromptData : PromptPtrs)
        {
            if (PromptData)
            {
                PromptsForTopic.Add(*PromptData);
            }
        }
        FConversationData TopicConversationData;
        TopicConversationData.ConversationPromptArray = PromptsForTopic;
        ConversationData.Add(TopicConversationData);
    }
}

void UDialogComponent::UpdatePromptAtIndex(int32 TopicIndex, int32 PromptIndex)
{
    auto TopicData = ConversationData[TopicIndex];
    const int32 LastValidPromptIndex = TopicData.ConversationPromptArray.Num() - 1;
    auto PromptData = TopicData.ConversationPromptArray[PromptIndex];

    if (PromptData.SingleUse)
    {
        
    }
}

int UDialogComponent::ConversationCount() const
{
    int Count = 0;
    for (FConversationData Element : ConversationData)
    {
        if (Element.PromptsAvailableCount() > 0)
        {
            ++Count;
        }
    }
    return Count;
}

