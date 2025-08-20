// (c) 2025 Sarah Smith


#include "BarkText.h"

#include "../AdventureGame.h"
#include "BarkLine.h"
#include "BarkRequest.h"
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

    if (!IsHidden && IsBarking)
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
    RequestQueue.Add(BarkRequest);
    SetBarkLineTimer();
}

void UBarkText::SetText(FText NewText)
{
    APlayerController* PlayerController = GetOwningPlayer();
    check(PlayerController);
    check(BarkLineClass);
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
}

void UBarkText::ClearText()
{
    HideContainer();
    ClearBarkLineTimer();
    BarkContainer->ClearChildren();
    BarkLines.Empty();
    bWarningShown = false;
    CurrentUID = -1;
    CurrentBarkLine = 0;
}

void UBarkText::HideContainer()
{
    BarkContainer->SetVisibility(ESlateVisibility::Hidden);
    IsHidden = true;
}

void UBarkText::ShowContainer()
{
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
    CurrentBarkRequest = RequestQueue[0];
    RequestQueue.RemoveAt(0);
    BarkLines = CurrentBarkRequest->GetBarkLines();
    CurrentUID = CurrentBarkRequest->GetUID();
    BarkPosition = CurrentBarkRequest->GetPosition();
    BarkLineDisplayTime = CurrentBarkRequest->GetDuration();
    BarkTextColor = CurrentBarkRequest->GetColor();
}

void UBarkText::AddQueuedBarkLine()
{
    if (!IsLastBarkLine())
    {
        SetText(BarkLines[++CurrentBarkLine]);
        SetBarkLineTimer();
    }
    else
    {
        if (CurrentBarkRequest)
        delete CurrentBarkRequest;
        CurrentBarkRequest = nullptr;
        BarkRequestCompleteDelegate.Broadcast(CurrentUID);
        ClearText();
        if (RequestQueue.Num() > 0)
        {
            HideContainer();
            LoadNextBarkRequest();
            SetBarkLineTimer();
        }
    }
}

void UBarkText::SetBarkLineTimer()
{
    BarkLineTimer = CurrentBarkRequest ? CurrentBarkRequest->GetDurationForLine(CurrentBarkLine) : BarkLineDisplayTime;
    IsBarking = true;
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
