// (c) 2025 Sarah Smith


#include "DialogComponent.h"

#include "BarkText.h"
#include "ConversationData.h"
#include "HotSpotNPC.h"
#include "../HUD/PromptList.h"
#include "../HUD/AdventureGameHUD.h"

#include "../AdventureGame.h"
#include "../Player/AdventurePlayerController.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDialogComponent::UDialogComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    
}


// Called when the game starts
void UDialogComponent::BeginPlay()
{
    Super::BeginPlay();

    FillConversationData();
    ConversationDataLoad.ExecuteIfBound(this);
}

void UDialogComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    ConversationDataSave.ExecuteIfBound(this);
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
#if WITH_EDITOR
        FString ErrorMessage;
        bool IsOK = TopicConversationData.Validate(ErrorMessage);
        if (!IsOK && !ErrorMessage.IsEmpty())
        {
            GEngine->AddOnScreenDebugMessage(1, 10.0, FColor::Red,
                    *ErrorMessage,false, FVector2D(2.0, 2.0));
            UE_LOG(LogAdventureGame, Error, TEXT("Error in Conversation: check tables: %s"), *ErrorMessage);
        }
#endif
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
    PromptList->PromptClickedEvent.RemoveAll(this);
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
            UE_LOG(LogAdventureGame, Warning, TEXT("Assigning new conversation topic - %d"), TopicIndex);
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
    PromptList->PromptEntries[CurrentPromptIndex]->FlashPrompt();
    UpdatePromptAtIndex(TopicIndex, CurrentPromptIndex);
    ShowPlayerBark();
}

void UDialogComponent::DisplayPrompts()
{
    UE_LOG(LogAdventureGame, Warning, TEXT("DisplayPrompts"));
    PromptList->SetPromptMenusEnabled(true);
    const int MaxPrompts = std::min(PromptList->PromptEntries.Num(), PromptsToShow.Num());
    if (MaxPrompts <= 0)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("Prompts are empty - aborting conversation"));
        StopMonitoringConversations();
        return;
    }
    PromptList->HidePromptEntries();
    for (int i = 0; i < MaxPrompts; ++i)
    {
        PromptList->SetPromptText(PromptsToShow[i].PromptText[0], PromptsToShow[i].HasBeenSelected, i);
    }
    DialogState = EDialogState::Prompts;
}

void UDialogComponent::ShowPlayerBark()
{
    DialogState = EDialogState::Player;
    const FBarkRequest* PlayerRequest = FBarkRequest::CreatePlayerMultilineRequest(
        PromptsToShow[CurrentPromptIndex].PromptText);
    BarkUID = PlayerRequest->GetUID();
    Bark->AddBarkRequest(PlayerRequest);
    UE_LOG(LogAdventureGame, Warning, TEXT("UDialogComponent::ShowPlayerBark - UID: %d - %s"), BarkUID,
        *(PromptsToShow[CurrentPromptIndex].PromptText[0].ToString()));
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
    UE_LOG(LogAdventureGame, Warning, TEXT("ShowNextDialogPrompts - current prompt: %d - %s"), CurrentPromptIndex,
        *(PromptsToShow[CurrentPromptIndex].PromptText[0].ToString()));
    if (PromptsToShow[CurrentPromptIndex].EndsConversation)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("EndsConversation"));
        if (!PopConversationTopic())
        {
            UE_LOG(LogAdventureGame, Warning, TEXT("EndsConversation - exiting"));
            StopMonitoringConversations();
            return;
        }
    }
    else if (PromptsToShow[CurrentPromptIndex].SwitchTopic)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("Not: EndsConversation - SwitchTopic"));
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
    UE_LOG(LogAdventureGame, Warning, TEXT("PushConversationTopic"));
    Stack.Push(TopicIndex);
    const UDataTable* NewTopic = PromptsToShow[CurrentPromptIndex].SwitchTopic;
    AssignNewTopic(NewTopic);
}
