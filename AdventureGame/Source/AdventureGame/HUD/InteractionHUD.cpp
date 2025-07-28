// (c) 2025 Sarah Smith


#include "InteractionHUD.h"

#include "../Constants.h"
#include "Internationalization/StringTableRegistry.h"

void UInteractionHUD::SetText(const FText NewText)
{
    if (!TextLocked)
    {
        InteractionDescription->SetText(NewText);
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
    SetText(LOCTABLE(ITEM_DESCRIPTIONS_KEY, G_WALK_TO_KEY));
}
