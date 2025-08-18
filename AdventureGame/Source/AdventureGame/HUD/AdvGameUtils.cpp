#include "AdvGameUtils.h"

#include "../Constants.h"
#include "../HotSpots/HotSpot.h"
#include "../Items/InventoryItem.h"

/**
 * Test to see if Current has changed more than DBL_EPSILON compared to Previous in either `X` or `Y`.
 * @param Current `FVector2D` Value to check to see if it differs
 * @param Previous `FVector2D` Value to check against as a baseline
 * @return true if `Current` is significantly different to `Previous`, false otherwise
 */
bool AdvGameUtils::HasChangedMuch(const FVector2D& Current, const FVector2D& Previous)
{
    return (fabs(Previous.X - Current.X) >= std::numeric_limits<double>::epsilon() || fabs(Previous.Y - Current.Y) >=
        std::numeric_limits<double>::epsilon());
}

int32 AdvGameUtils::GetUUID()
{
    uuid_t UUIDLatent;
    uuid_generate(UUIDLatent);
    return UUIDLatent[0] | UUIDLatent[1] << 8 | UUIDLatent[2] << 16 | UUIDLatent[3] << 24;
}

FText AdvGameUtils::GetGivingItemText(const UInventoryItem* CurrentItem, const UInventoryItem* TargetItem,
                                      const AHotSpot* HotSpot)
{
    verifyf(CurrentItem, TEXT("GetGivingItemString expects CurrentItem to be non-null"));
    verifyf(TargetItem || HotSpot, TEXT("GetGivingItemString expects either TargetItem or HotSpot to be non-null"));
    FText SubjectText = CurrentItem->ShortDescription;
    FText TargetText = HotSpot ? HotSpot->ShortDescription : TargetItem->ShortDescription;
    FFormatNamedArguments VerbArgs;
    VerbArgs.Add("Subject", SubjectText);
    VerbArgs.Add("Object", TargetText);
    return FText::Format(LOCTABLE(ITEM_DESCRIPTIONS_KEY, G_GIVE_SUBJECT_TO_OBJECT_KEY), VerbArgs);
}

FText AdvGameUtils::GetUsingItemText(const UInventoryItem* CurrentItem, const UInventoryItem* TargetItem,
                                     const AHotSpot* HotSpot)
{
    verifyf(CurrentItem, TEXT("GetGivingItemString expects CurrentItem to be non-null"));
    verifyf(TargetItem || HotSpot, TEXT("GetGivingItemString expects either TargetItem or HotSpot to be non-null"));
    FText SubjectText = CurrentItem->ShortDescription;
    FText TargetText = HotSpot ? HotSpot->ShortDescription : TargetItem->ShortDescription;
    FFormatNamedArguments VerbArgs;
    VerbArgs.Add("Subject", SubjectText);
    VerbArgs.Add("Object", TargetText);
    return FText::Format(LOCTABLE(ITEM_DESCRIPTIONS_KEY, G_USE_SUBJECT_ON_OBJECT_KEY), VerbArgs);
}

FText AdvGameUtils::GetVerbWithItemText(const UInventoryItem* CurrentItem, const EVerbType Verb)
{
    verifyf(CurrentItem, TEXT("GetVerbWithItemString expects CurrentItem to be non-null"));
    const FText ItemText = FItemKind::GetDescription(CurrentItem->ItemKind);
    FFormatNamedArguments VerbArgs;
    VerbArgs.Add("Verb", VerbGetDescriptiveString(Verb));
    VerbArgs.Add("Subject", ItemText);
    return FText::Format(LOCTABLE(ITEM_DESCRIPTIONS_KEY, G_VERB_SUBJECT_KEY), VerbArgs);
}

FText AdvGameUtils::GetVerbWithHotSpotText(const AHotSpot* HotSpot, const EVerbType Verb)
{
    verifyf(HotSpot, TEXT("GetVerbWithHotSpotString expects HotSpot to be non-null"));
    FFormatNamedArguments VerbArgs;
    VerbArgs.Add("Verb", VerbGetDescriptiveString(Verb));
    VerbArgs.Add("Subject", HotSpot->ShortDescription);
    return FText::Format(LOCTABLE(ITEM_DESCRIPTIONS_KEY, G_VERB_SUBJECT_KEY), VerbArgs);
}

TArray<FText> AdvGameUtils::NewLineSeperatedToArrayText(const FText& NewText)
{
    FString Line = NewText.ToString();
    if (!Line.Contains(NEW_LINE_SEPARATOR)) return TArray<FText>({ NewText });
    TArray<FString> NewLines = NewLineSeperatedToArrayString(Line);
    TArray<FText> OutText;
    Algo::Transform(NewLines, OutText, [](const FString& Str){ return FText::FromString(Str); });
    return OutText;
}

TArray<FString> AdvGameUtils::NewLineSeperatedToArrayString(const FString& NewString)
{
    if (NewString.Contains(NEW_LINE_SEPARATOR))
    {
        TArray<FString> NewLines;
        FString First, Rest, Buf = NewString;
        while (Buf.Split(NEW_LINE_SEPARATOR, &First, &Rest))
        {
            NewLines.Add(First.TrimStartAndEnd());
            Buf = Rest;
        }
        NewLines.Add(Buf.TrimStartAndEnd());
        return NewLines;
    }
    return TArray({ NewString });
}

TArray<FText> AdvGameUtils::WrapTextLinesToMaxCharacters(const FText& NewText, uint32 MaxLength)
{
    TArray<FText> WrappedLines;
    uint32 CurrentStringIndex = 0;
    FString CurrentLine = NewText.ToString();
    while (CurrentLine.Len() > MaxLength)
    {
        int32 SplitPoint = MaxLength;
        while (CurrentLine[--SplitPoint] != ' ' && SplitPoint > 0) {}
        if (SplitPoint < 0) SplitPoint = MaxLength; // Did not find a space to split at
        WrappedLines.Add(FText::FromString(CurrentLine.Left(SplitPoint)));
        CurrentLine = CurrentLine.Mid(SplitPoint).TrimStartAndEnd();
    }
    WrappedLines.Add(FText::FromString(CurrentLine));
    return WrappedLines;
}
