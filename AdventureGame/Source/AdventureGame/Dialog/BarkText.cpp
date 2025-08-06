// (c) 2025 Sarah Smith


#include "BarkText.h"

#include "../AdventureGame.h"
#include "BarkLine.h"
#include "AdventureGame/Constants.h"
#include "AdventureGame/HUD/AdvGameUtils.h"

#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void UBarkText::NativeOnInitialized()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("BarkText Initialized"));
}

void UBarkText::SetText(FText NewText)
{
	if (NewText.IsEmpty()) return;
	if (NewText.ToString().Contains(NEW_LINE_SEPARATOR))
	{
		auto Lines = AdvGameUtils::NewLineSeperatedToArrayText(NewText);
		SetBarkLines(Lines);
		return;
	}
	APlayerController* PlayerController = GetOwningPlayer();
	check(PlayerController);
	check(BarkLineClass);
	UE_LOG(LogAdventureGame, Warning, TEXT("SetText: %s ========"), *NewText.ToString());
	DumpBarkText();
	const int LineCount = BarkContainer->GetChildrenCount();
	FName LineName(FString::Printf(TEXT("BarkLine_%d"), LineCount));
	UUserWidget *Widget = CreateWidget(PlayerController, BarkLineClass, LineName);
	if (UBarkLine *BarkLine = Cast<UBarkLine>(Widget))
	{
		BarkLine->Text->SetText(NewText);
		BarkLine->Text->SetColorAndOpacity(BarkTextColor);
		BarkContainer->AddChildToVerticalBox(Widget);
	}
	DumpBarkText();
	UE_LOG(LogAdventureGame, Warning, TEXT("SetText: %s ========"), *NewText.ToString());
}

void UBarkText::ClearText()
{
	ClearBarkLineTimer();
	BarkContainer->ClearChildren();
	BarkLines.Empty();
}

void UBarkText::SetBarkLines(const TArray<FText> &NewBarkLines)
{
	ClearText();
	if (NewBarkLines.Num() == 0) return;
	SetText(NewBarkLines[0]);
	if (NewBarkLines.Num() > 1)
	{
		auto It = NewBarkLines.begin();
		for ( ++It; It != NewBarkLines.end(); ++It)
		{
			BarkLines.Add(*It);
		}
		SetBarkLineTimer();
	}
}

void UBarkText::AddQueuedBarkLine()
{
	if (BarkLines.Num() > 0)
	{
		FText NextLine = BarkLines[0];
		SetText(NextLine);
		BarkLines.RemoveAt(0);
		if (BarkLines.Num() > 0)
		{
			SetBarkLineTimer();
		}
	}
}

void UBarkText::SetBarkLineTimer()
{
	APlayerController* PlayerController = GetOwningPlayer();
	PlayerController->GetWorldTimerManager().SetTimer(
		BarkLineTimer, this, &UBarkText::AddQueuedBarkLine,
		1.0f, false, BARK_LINE_DELAY
		);
}

void UBarkText::ClearBarkLineTimer()
{
	APlayerController* PlayerController = GetOwningPlayer();
	PlayerController->GetWorldTimerManager().ClearTimer(BarkLineTimer);
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
			UE_LOG(LogAdventureGame, Log, TEXT("Widget display: %d - %s"), i, *(LineText.ToString()));
		}
		i++;
	}

	for (FText Text : BarkLines)
	{
		UE_LOG(LogAdventureGame, Log, TEXT("Queued Barkline: %d - %s"), i, *(Text.ToString()));
	}
}
