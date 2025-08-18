// (c) 2025 Sarah Smith


#include "HotSpotNPC.h"

#include "DialogComponent.h"
#include "PaperFlipbookComponent.h"
#include "../Player/AdventurePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHotSpotNPC::AHotSpotNPC()
{
    PrimaryActorTick.bCanEverTick = false;

    FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
    FlipbookComponent->SetupAttachment(RootComponent);
    FlipbookComponent->SetLooping(true);
    FlipbookComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));

    DialogComponent = CreateDefaultSubobject<UDialogComponent>("DialogComponent");

    if (DefaultFlipbook)
    {
        FlipbookComponent->SetFlipbook(DefaultFlipbook);
    }
}

EVerbType AHotSpotNPC::CheckForDefaultCommand() const
{
    if (DialogComponent->ConversationCount() > 0)
    {
        return EVerbType::TalkTo;
    }
    return Super::CheckForDefaultCommand();
}

// Called when the game starts or when spawned
void AHotSpotNPC::BeginPlay()
{
    Super::BeginPlay();
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
    TArray<FPromptData> PromptsToShow;
    DialogComponent->LoadPrompts(PromptsToShow);
    if (const AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(PC); IsValid(Apc))
    {
        Apc->AdventureHUDWidget->ShowPromptList();
        UPromptList *Prompts = Apc->AdventureHUDWidget->PromptList;
        const int MaxPrompts = std::min(Prompts->PromptEntries.Num(),PromptsToShow.Num());
        for (int i = 0; i < MaxPrompts; ++i)
        {
            Prompts->SetPromptText(PromptsToShow[i].PromptText[0], PromptsToShow[i].HasBeenSelected, i);
        }
    }
}

void AHotSpotNPC::OnTalkTo_Implementation()
{
    Execute_OnConverseWith(this);
}
