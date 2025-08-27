// (c) 2025 Sarah Smith


#include "BarkText.h"

#include "../AdventureGame.h"
#include "BarkLine.h"
#include "BarkRequest.h"
#include "../Player/AdventureCharacter.h"
#include "../Player/AdventurePlayerController.h"
#include "Components/SphereComponent.h"

#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void UBarkText::NativeOnInitialized()
{
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("BarkText Initialized"));
    HideContainer();
    IsBarking = false;
    ViewTarget = nullptr;
}

void UBarkText::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (IsBarking && IsRenderTransitionSet && IsOneLineAtMinimumSet)
    {
        ShowContainer();
    }

    if (IsBarking)
    {
        if (ViewTarget == nullptr)
        {
            const APlayerController* PlayerController = GetOwningPlayer();
            ViewTarget = PlayerController->GetViewTarget();
        }
        if (ViewTarget)
        {
            const FVector CameraLocation = ViewTarget->GetActorLocation();
            const FVector Offset = BarkPosition->GetComponentLocation() - CameraLocation;
            SetRenderTranslation(FVector2D(Offset.X, Offset.Y));
            IsRenderTransitionSet = true;
        }
    }

    if (IsBarking)
    {
        BarkLineTimer -= InDeltaTime;
        if (BarkLineTimer <= 0.f)
        {
            AddQueuedBarkLine();
        }
    }
}

void UBarkText::NativeDestruct()
{
    ClearBarkQueue();
    Super::NativeDestruct();
}

void UBarkText::AddBarkRequest(const FBarkRequest *BarkRequest)
{
    // UE_LOG(LogAdventureGame, Warning, TEXT("AddBarkRequest"));
    // FBarkRequest::Dump(const_cast<FBarkRequest *>(BarkRequest));
    AddToLinkedList(BarkRequest);
    if (!IsBarking)
    {
        LoadNextBarkRequest();
        IsBarking = true;
        BarkLineTimer = 0.1f;
    }
}

void UBarkText::SetText(const FText &NewText)
{
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT(">> ============== SetText ================"));
    APlayerController* PlayerController = GetOwningPlayer();
    check(PlayerController);
    check(BarkLineClass);
    DumpBarkText();
    if (BarkContainer->GetChildrenCount() >= G_MAX_BARK_LINES)
    {
        UWidget *OldChild = BarkContainer->GetChildAt(0);
        BarkContainer->ShiftChild(2, OldChild);
        UBarkLine *BarkLine = Cast<UBarkLine>(OldChild);
        BarkLine->Text->SetText(NewText);
        return;
    }
    UUserWidget* Widget = CreateWidget(PlayerController, BarkLineClass);
    if (UBarkLine* BarkLine = Cast<UBarkLine>(Widget))
    {
        BarkLine->Text->SetText(NewText);
        BarkLine->Text->SetColorAndOpacity(BarkTextColor);
        BarkContainer->AddChildToVerticalBox(Widget);
    }
    IsOneLineAtMinimumSet = true;
    DumpBarkText();
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("<< ============== SetText ================"));
}

void UBarkText::ClearText()
{
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("ClearText"));
    HideContainer();
    ClearBarkLineTimer();
    BarkContainer->ClearChildren();
    BarkLines.Empty();
    bWarningShown = false;
    CurrentUID = -1;
    CurrentBarkLine = 0;
    IsRenderTransitionSet = false;
    IsOneLineAtMinimumSet = false;
}

void UBarkText::HideContainer()
{
    if (IsHidden) return;
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("HideContainer"));
    BarkContainer->SetVisibility(ESlateVisibility::Hidden);
    IsHidden = true;
}

void UBarkText::ShowContainer()
{
    if (!IsHidden) return;
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("ShowContainer"));
    BarkContainer->SetVisibility(ESlateVisibility::Visible);
    IsHidden = false;
}

void UBarkText::SetBarkLines(const TArray<FText>& NewBarkLines)
{
    ClearText();
    if (NewBarkLines.Num() == 0) return;
    SetText(NewBarkLines[0]);
    if (NewBarkLines.Num() > 1)
    {
        auto It = NewBarkLines.begin();
        for (++It; It != NewBarkLines.end(); ++It)
        {
            const FText& Line = *It;
            BarkLines.Add(*It);
        }
        SetBarkLineTimer();
    }
}

void UBarkText::LoadNextBarkRequest()
{
    CurrentBarkRequest = PopBarkRequest();
    if (!CurrentBarkRequest)
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("Bark Request Loading Failed"));
        return;
    }
    BarkLines.Empty();
    CurrentBarkRequest->GetBarkLines(BarkLines);
    CurrentUID = CurrentBarkRequest->GetUID();
    UE_LOG(LogAdventureGame, Warning, TEXT("LoadNextBarkRequest: %d - %s"), CurrentUID, *BarkLines[0].ToString());
    BarkPosition = CurrentBarkRequest->GetPosition();
    if (!IsValid(BarkPosition))
    {
        const AAdventurePlayerController* PlayerController = Cast<AAdventurePlayerController>(GetOwningPlayer());
        BarkPosition = PlayerController->PlayerCharacter->Sphere;
    }
    BarkLineDisplayTime = CurrentBarkRequest->GetDuration();
    BarkTextColor = CurrentBarkRequest->GetColor();
}

void UBarkText::AddQueuedBarkLine()
{
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AddQueuedBarkLine - count: %d - current: %d"), BarkLines.Num(), CurrentBarkLine);
    if (CurrentBarkLine < BarkLines.Num())
    {
        HideContainer();
        IsRenderTransitionSet = false;
        IsOneLineAtMinimumSet = false;
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AddQueuedBarkLine: %d %s"), CurrentBarkLine, *BarkLines[CurrentBarkLine].ToString())
        SetText(BarkLines[CurrentBarkLine]);
        SetBarkLineTimer();
        CurrentBarkLine++;
    }
    else
    {
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AddQueuedBarkLine - no more lines - doing clean up"));
        if (CurrentBarkRequest)
        delete CurrentBarkRequest;
        CurrentBarkRequest = nullptr;
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AddQueuedBarkLine - UID %d, broadcasting - doing clean up"), CurrentUID);
        BarkRequestCompleteDelegate.Broadcast(CurrentUID);
        HideContainer();
        ClearText();
        if (RequestQueue)
        {
            LoadNextBarkRequest();
            SetBarkLineTimer();
        }
        else
        {
            UE_LOG(LogAdventureGame, Warning, TEXT("AddQueuedBarkLine - request queue empty stop barking"));
            IsBarking = false;
        }
    }
}

void UBarkText::SetBarkLineTimer()
{
    BarkLineTimer = CurrentBarkRequest ? CurrentBarkRequest->GetDurationForLine(CurrentBarkLine) : BarkLineDisplayTime;
    IsBarking = true;
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("#### SetBarkLineTimer: %f"), BarkLineTimer);
}

void UBarkText::ClearBarkLineTimer()
{
    BarkLineTimer = 0;
    IsBarking = false;
}

void UBarkText::DumpBarkText()
{
    auto Children = BarkContainer->GetAllChildren();
    int i = 0;
    for (UWidget* Child : Children)
    {
        if (const auto BarkLine = Cast<UBarkLine>(Child))
        {
            FText LineText = BarkLine->Text->GetText();
            UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Widget display: %d - %s"), i, *(LineText.ToString()));
        }
        i++;
    }

    for (FText Text : BarkLines)
    {
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Queued Barkline: %d - %s"), i, *(Text.ToString()));
    }
}
