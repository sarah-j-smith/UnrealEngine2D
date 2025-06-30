// (c) 2025 Sarah Smith


#include "BarkText.h"

#include "AdventureGame.h"
#include "Components/TextBlock.h"

void UBarkText::NativeOnInitialized()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("BarkText Initialized"));
	if (!IsValid(BaseText)|| !IsValid(OutlineText) ) return;
	BaseText->SetAutoWrapText(true);
	OutlineText->SetAutoWrapText(true);
}

void UBarkText::SetText(FText NewText)
{
	if (!IsValid(BaseText) || !IsValid(OutlineText) ) return;
	BaseText->SetText(NewText);
	OutlineText->SetText(NewText);
}

FVector2D UBarkText::GetSize()
{
	return FVector2D();
}

