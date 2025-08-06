// (c) 2025 Sarah Smith


#include "HotSpotNPC.h"
#include "PaperFlipbookComponent.h"
#include "../Player/AdventurePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHotSpotNPC::AHotSpotNPC()
{
    PrimaryActorTick.bCanEverTick = false;

    FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
    FlipbookComponent->SetupAttachment(RootComponent);

    if (DefaultFlipbook)
    {
        FlipbookComponent->SetFlipbook(DefaultFlipbook);
    }
}

// Called when the game starts or when spawned
void AHotSpotNPC::BeginPlay()
{
    Super::BeginPlay();

    ConversationDataLoad.Execute(this);
    FillConversationData();
}

// Called every frame
void AHotSpotNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AHotSpotNPC::OnConverseWith_Implementation()
{
    IDialogInteractable::OnConverseWith_Implementation();
    APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (AAdventurePlayerController *APC = Cast<AAdventurePlayerController>(PC); IsValid(APC))
    {
        // APC->AdventureHUDWidget->ShowBlackScreen()
    }
}

void AHotSpotNPC::FillConversationData()
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

void AHotSpotNPC::UpdatePromptAtIndex(int32 TopicIndex, int32 PromptIndex)
{
    auto TopicData = ConversationData[TopicIndex];
    const int32 LastValidPromptIndex = TopicData.ConversationPromptArray.Num() - 1;
    auto PromptData = TopicData.ConversationPromptArray[PromptIndex];

    if (PromptData.SingleUse)
    {
        
    }
}

