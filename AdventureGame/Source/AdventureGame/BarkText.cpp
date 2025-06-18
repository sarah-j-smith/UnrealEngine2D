// (c) 2025 Sarah Smith


#include "BarkText.h"

#include "AdventureGame.h"
#include "Components/TextBlock.h"

void UBarkText::NativeOnInitialized()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("BarkText Initialized"));
}

void UBarkText::SetText(FString NewText)
{
	BaseText->SetText(FText::FromString(NewText));
	OutlineText->SetText(FText::FromString(NewText));
}
