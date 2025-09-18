// (c) 2025 Sarah Smith


#include "HotSpotNPC.h"

#include "DialogComponent.h"
#include "PaperFlipbookComponent.h"
#include "../Player/AdventurePlayerController.h"
#include "../HUD/AdventureGameHUD.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AHotSpotNPC::AHotSpotNPC()
{
    PrimaryActorTick.bCanEverTick = false;

    // This is not a "real" sphere - its not a mesh, its just a collision volume defined by dimensions
    BarkPosition = CreateDefaultSubobject<USphereComponent>(TEXT("BarkPosition"));
    BarkPosition->SetupAttachment(RootComponent);
    BarkPosition->SetSphereRadius(4.0f);

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

    DialogComponent->ConversationEndedEvent.AddUObject(this, &AHotSpotNPC::OnConversationComplete);
}

// Called every frame
void AHotSpotNPC::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsConversing && ShouldStopConversation)
    {
        ShouldStopConversation = true;
        StopConversation();
    }
}

void AHotSpotNPC::OnConverseWith_Implementation()
{
    IDialogInteractable::OnConverseWith_Implementation();
    APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(PC); IsValid(Apc))
    {
        Apc->AdventureHUDWidget->ShowPromptList();
        Apc->SetInputLocked(true);
        IsConversing = true;
        DialogComponent->HandleConversations();
    }
}

void AHotSpotNPC::OnTalkTo_Implementation()
{
    Execute_OnConverseWith(this);
}

void AHotSpotNPC::OnConversationComplete()
{
    UE_LOG(LogTemp, Warning, TEXT("OnConversationComplete"));
    ShouldStopConversation = true;
}

void AHotSpotNPC::StopConversation()
{
    UE_LOG(LogTemp, Warning, TEXT("StopConversation - IsConversing: %s"), IsConversing ? TEXT("Yes") : TEXT("No"));
    if (!IsConversing) return;
    UE_LOG(LogTemp, Warning, TEXT("Stopping Conversation"));
    APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (AAdventurePlayerController *Apc = Cast<AAdventurePlayerController>(PC); IsValid(Apc))
    {
        Apc->AdventureHUDWidget->HidePromptList();
        Apc->SetInputLocked(false);
        DialogComponent->HandleConversations();
    }
    IsConversing = false;
}
