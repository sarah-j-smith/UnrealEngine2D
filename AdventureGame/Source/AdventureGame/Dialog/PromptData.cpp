// (c) 2025 Sarah Smith


#include "PromptData.h"

bool FPromptData::CanBeShown() const
{
    // If a prompt is SingleUse and has been selected already then we can't show it.
    if (SingleUse && HasBeenSelected) return false;

    // If its otherwise not a SingleUse prompt that has already been shown, then
    // it can be shown if its visible.
    return Visible;
}
