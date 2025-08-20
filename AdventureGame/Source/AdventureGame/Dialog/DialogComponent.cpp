// (c) 2025 Sarah Smith


#include "DialogComponent.h"

#include "BarkText.h"
#include "ConversationData.h"
#include "HotSpotNPC.h"
#include "../HUD/PromptList.h"
#include "AdventureGame/AdventureGame.h"
#include "AdventureGame/Gameplay/AdvBlueprintFunctionLibrary.h"
#include "AdventureGame/HUD/AdvGameUtils.h"
#include "AdventureGame/Player/AdventurePlayerController.h"
#include "Kismet/GameplayStatics.h"

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
        for (const FPromptData* PromptData : PromptPtrs)
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

void UDialogComponent::LoadPrompts(TArray<FPromptData>& PromptsToShow)
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

void UDialogComponent::HandleConversations()
{
    InitialiseForBarking();
    // Get the prompts and display them on the buttons, wait for the player to click
    PromptsToShow.Empty();
    LoadPrompts(PromptsToShow);
    DisplayPrompts();
    PromptList->PromptClickedEvent.AddDynamic(this, &UDialogComponent::HandlePromptClick);
}

void UDialogComponent::StopMonitoringConversations()
{
    TearDownBarkingSetup();
    PromptsToShow.Empty();
    DialogState = EDialogState::Hidden;
    ConversationEndedEvent.Broadcast();
}

void UDialogComponent::AssignNewTopic(const UDataTable* NewTopic)
{
    int IndexToSet = 0;
    if (!IsValid(NewTopic)) return;
    for (const UDataTable* Topic : TopicList)
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
    CurrentPromptIndex = PromptIndex - 1; // Prompts are numbered 1 - 5
    PromptList->SetPromptMenusEnabled(false);
    UpdatePromptAtIndex(PromptIndex, PromptIndex);
    ShowPlayerBark();
}

void UDialogComponent::DisplayPrompts()
{
    PromptList->SetPromptMenusEnabled(true);
    const int MaxPrompts = std::min(PromptList->PromptEntries.Num(), PromptsToShow.Num());
    if (MaxPrompts <= 0)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("Prompts are empty - aborting conversation"));
        StopMonitoringConversations();
        return;
    }
    for (int i = 0; i < MaxPrompts; ++i)
    {
        PromptList->SetPromptText(PromptsToShow[i].PromptText[0], PromptsToShow[i].HasBeenSelected, i);
    }
    DialogState = EDialogState::Prompts;
}

void UDialogComponent::ShowPlayerBark()
{
    UE_LOG(LogAdventureGame, Warning, TEXT("ShowPlayerBark"));
    DialogState = EDialogState::Player;
    const FBarkRequest* PlayerRequest = FBarkRequest::CreatePlayerMultilineRequest(
        PromptsToShow[CurrentPromptIndex].PromptText);
    BarkUID = PlayerRequest->GetUID();
    Bark->AddBarkRequest(PlayerRequest);
}

void UDialogComponent::ShowNPCResponse()
{
    UE_LOG(LogAdventureGame, Warning, TEXT("ShowNPCResponse"));
    DialogState = EDialogState::NPC;
    const AHotSpotNPC* NPC = Cast<AHotSpotNPC>(GetOwner());
    check(NPC);
    const FBarkRequest* NPCRequest = FBarkRequest::CreateNPCMultilineRequest(
        PromptsToShow[CurrentPromptIndex].NPCResponse, 0,
        NPC->BarkPosition, TextColor.ToFColor(true));
    BarkUID = NPCRequest->GetUID();
    Bark->AddBarkRequest(NPCRequest);
}

void UDialogComponent::ShowNextDialogPrompts()
{
    if (PromptsToShow[CurrentPromptIndex].EndsConversation)
    {
#if WITH_EDITOR
        if (PromptsToShow[CurrentPromptIndex].SwitchTopic)
        {
            const FString PromptText = PromptsToShow[CurrentPromptIndex].PromptText[0].ToString().Left(20);
            UE_LOG(LogAdventureGame, Error, TEXT("Ignoring switch topic on prompt %s - because it ends conversation"),
                   *PromptText);
        }
#endif
        if (!PopConversationTopic())
        {
            StopMonitoringConversations();
            return;
        }
    }
    else if (PromptsToShow[CurrentPromptIndex].SwitchTopic)
    {
        PushConversationTopic();
    }
    PromptsToShow.Empty();
    LoadPrompts(PromptsToShow);
    DisplayPrompts();
}

void UDialogComponent::InitialiseForBarking()
{
    NPC = Cast<AHotSpotNPC>(GetOwner());
    check(NPC);
    Apc = Cast<AAdventurePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    check(Apc);
    PromptList = Apc->AdventureHUDWidget->PromptList;
    Bark = Apc->AdventureHUDWidget->Bark;
    BarkUID = AdvGameUtils::GetUUID();
    Apc->ClearBark(true);
    Bark->BarkRequestCompleteDelegate.AddUObject(this, &UDialogComponent::OnBarkTimerTimeout);
}

void UDialogComponent::TearDownBarkingSetup()
{
    if (Bark)
    {
        Bark->BarkRequestCompleteDelegate.RemoveAll(this);
    }
    if (PromptList)
    {
        PromptList->PromptClickedEvent.RemoveAll(this);
    }
    BarkUID = -1;
    Apc = nullptr;
    Bark = nullptr;
    PromptList = nullptr;
}

void UDialogComponent::OnBarkTimerTimeout(int32 UID)
{
    // Triggered by the Player Controller's Bark Timeout.
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("OnBarkTimerTimeout: Expected UID: %d vs Got UID: %d"),
           BarkUID, UID);
    if (UID == BarkUID)
    {
        // Only react if its our text that caused the timeout
        if (DialogState == EDialogState::Player)
        {
            ShowNPCResponse();
        }
        else if (DialogState == EDialogState::NPC)
        {
            ShowNextDialogPrompts();
        }
    }
}

bool UDialogComponent::PopConversationTopic()
{
    if (Stack.Num() > 0)
    {
        TopicIndex = Stack.Pop();
        return true;
    }
    return false;
}

void UDialogComponent::PushConversationTopic()
{
    Stack.Push(TopicIndex);
    const UDataTable* NewTopic = PromptsToShow[TopicIndex].SwitchTopic;
    AssignNewTopic(NewTopic);
}
