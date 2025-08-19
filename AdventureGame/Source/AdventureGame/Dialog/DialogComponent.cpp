// (c) 2025 Sarah Smith


#include "DialogComponent.h"

#include "BarkText.h"
#include "ConversationData.h"
#include "HotSpotNPC.h"
#include "../HUD/PromptList.h"
#include "AdventureGame/AdventureGame.h"
#include "AdventureGame/Gameplay/AdvBlueprintFunctionLibrary.h"
#include "AdventureGame/Player/AdventurePlayerController.h"

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

    BarkTimerDelegate.BindUObject(this, &UDialogComponent::OnBarkTimerTimeout);
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
    int PromptSubIndex = PromptsToShow[PromptIndex].PromptSubNumber;
    ConversationData[TopicIndex].MarkPromptSelected(PromptIndex, PromptSubIndex);
}

void UDialogComponent::LoadPrompts(TArray<FPromptData> &PromptsToShow)
{
    if (ConversationData.IsEmpty()) return;
    if (TopicIndex < ConversationData.Num() && TopicIndex >= 0)
    {
        ConversationData[TopicIndex].DisplayPrompts(PromptsToShow);
    }
    else
    {
        UE_LOG(LogAdventureGame, Warning,
            TEXT("TopicIndex %d is not in the bounds of conversation topics, 0 to %d"),
            TopicIndex, ConversationData.Num());
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

void UDialogComponent:: HandleConversations(UPromptList *Prompts, UBarkText *BarkText)
{
    PromptList = Prompts;
    BarkTextDisplay = BarkText;
    PromptsToShow.Empty();
    LoadPrompts(PromptsToShow);
    const int MaxPrompts = std::min(Prompts->PromptEntries.Num(),PromptsToShow.Num());
    if (MaxPrompts > 0)
    {
        Prompts->PromptClickedEvent.AddDynamic(this, &UDialogComponent::HandlePromptClick);
        for (int i = 0; i < MaxPrompts; ++i)
        {
            Prompts->SetPromptText(PromptsToShow[i].PromptText[0], PromptsToShow[i].HasBeenSelected, i);
        }
        DialogState = EDialogState::Prompts;
    }
    else
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("Prompts are empty"));
    }
}

void UDialogComponent::StopMonitoringConversations()
{
    GetOwner()->GetWorldTimerManager().ClearTimer(BarkTimerHandle);
    PromptList->PromptClickedEvent.RemoveAll(this);
    PromptList = nullptr;
    BarkTextDisplay->ClearText();
    PromptsToShow.Empty();
    DialogState = EDialogState::Hidden;
}

void UDialogComponent::AssignNewTopic(UDataTable* NewTopic)
{
    int IndexToSet = 0;
    if (!IsValid(NewTopic)) return;
    for (UDataTable* Topic : TopicList)
    {
        if (Topic == NewTopic)
        {
            TopicIndex = IndexToSet;
            return;
        }
        ++IndexToSet;
    }
    UE_LOG(LogAdventureGame, Error, TEXT("UDialogComponent::AssignNewTopic - Got bad topic"));
}

void UDialogComponent::HandlePromptClick(int PromptIndex)
{
    PromptList->SetPromptMenusEnabled(false);
    DialogState = EDialogState::Player;
    UpdatePromptAtIndex(PromptIndex, PromptIndex);
    const AHotSpotNPC *NPC = Cast<AHotSpotNPC>(GetOwner());
    check(NPC);
    check(BarkTextDisplay);
    float BarkTime = 0.0f;
    float PlayerBarkTime = 0.0f;
    for (const FText& NPCText : PromptsToShow[PromptIndex].NPCResponse)
    {
        PlayerBarkTime = UAdvBlueprintFunctionLibrary::GetBarkTime(NPCText.ToString());
    }
    for (const FText &PlayerText : PromptsToShow[PromptIndex].PromptText)
    {
        BarkTime += UAdvBlueprintFunctionLibrary::GetBarkTime(PlayerText.ToString());
    }
    AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(BarkTextDisplay->GetOwningPlayer());
    Apc->PlayerBark(PromptsToShow[PromptIndex].PromptText[0], FColor::White, PlayerBarkTime);
    BarkTextDisplay->SetBarkLines(PromptsToShow[PromptIndex].NPCResponse);

    // Calls OnBarkTimerTimeout -> ShowNextDialogPrompts
    NPC->GetWorldTimerManager().SetTimer(BarkTimerHandle, BarkTimerDelegate, 1.0, false, BarkTime);
}

void UDialogComponent::ShowNextDialogPrompts()
{
    PromptsToShow.Empty();
    LoadPrompts(PromptsToShow);
    const int MaxPrompts = std::min(PromptList->PromptEntries.Num(),PromptsToShow.Num());
    if (MaxPrompts > 0)
    {
        for (int i = 0; i < MaxPrompts; ++i)
        {
            PromptList->SetPromptText(PromptsToShow[i].PromptText[0], PromptsToShow[i].HasBeenSelected, i);
        }
    }
}

void UDialogComponent::OnBarkTimerTimeout()
{
    ShowNextDialogPrompts();
}

void UDialogComponent::PopConversationTopic()
{
}

