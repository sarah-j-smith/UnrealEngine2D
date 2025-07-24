// (c) 2025 Sarah Smith


#include "BarkText.h"

#include "AdventureGame.h"
#include "Components/TextBlock.h"

void UBarkText::NativeOnInitialized()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("BarkText Initialized"));
	if (!IsValid(Text)) return;
	Text->SetAutoWrapText(true);
}

void UBarkText::SetText(FText NewText)
{
	if (!IsValid(Text)) return;
	Text->SetText(NewText);
}
