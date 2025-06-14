// (c) 2025 Sarah Smith


#include "InteractionHUD.h"

void UInteractionHUD::SetText(FString NewText)
{
	if (!TextLocked)
	{
		InteractionDescription->SetText(FText::FromString(NewText));
	}
}

void UInteractionHUD::HighlightText()
{
	TextLocked = true;
	InteractionDescription->SetColorAndOpacity(TextHighlightColor);
}

void UInteractionHUD::UnhighlightText()
{
	TextLocked = false;
	InteractionDescription->SetColorAndOpacity(TextNormalColor);
}

void UInteractionHUD::ResetText()
{
	UnhighlightText();
	SetText("");
}
