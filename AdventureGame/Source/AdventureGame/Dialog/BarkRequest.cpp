#include "BarkRequest.h"

#include "AdventureGame/AdventureGame.h"
#include "AdventureGame/Gameplay/AdvBlueprintFunctionLibrary.h"
#include "AdventureGame/HUD/AdvGameUtils.h"

FBarkRequest::FBarkRequest(const TArray<FText>& NewBarkLines, const int32 UID,
                           const FColor Color, USphereComponent* Position,
                           const float Duration, bool IsPlayer)
    : BarkLines(NewBarkLines)
      , RequestUID(UID)
      , RequestColor(Color)
      , RequestPosition(Position)
      , RequestDuration(Duration)
      , IsPlayerRequest(IsPlayer)
{
}

FBarkRequest::FBarkRequest(const FText& NewBarkLine, const int32 UID,
                           const FColor Color, USphereComponent* Position,
                           const float Duration, const bool IsPlayer)
    : FBarkRequest(TArray({NewBarkLine}), UID, Color, Position, Duration, IsPlayer)
{
    //
}

FBarkRequest* FBarkRequest::CreatePlayerMultilineRequest(const TArray<FText>& NewBarkLines, float Duration, int32 UID)
{
    if (UID == -1)
    {
        UID = AdvGameUtils::GetUUID();
    }
    const FColor Color = G_Player_Default_Text_Colour.ToFColor(true);
#if WITH_EDITOR
    if (NewBarkLines.IsEmpty())
    {
        UE_LOG(LogAdventureGame, Warning, TEXT("Created player bark with blank text"));
        return new FBarkRequest(NewBarkLines, UID, Color, nullptr, Duration);
    }
#endif
    if (Duration == 0.0f)
    {
        for (const FText& Text : NewBarkLines)
        {
            Duration += UAdvBlueprintFunctionLibrary::GetBarkTime(Text.ToString());
        }
    }
    if (Algo::FindBy(NewBarkLines, true, [](const FText& Text) { return Text.ToString().Len() > BARK_LINE_WIDTH; }))
    {
        TArray<FText> ShortTexts;
        for (const FText& Text : NewBarkLines)
        {
            if (Text.ToString().Len() > BARK_LINE_WIDTH)
            {
                ShortTexts.Append(AdvGameUtils::WrapTextLinesToMaxCharacters(Text, BARK_LINE_WIDTH));
            }
            else
            {
                ShortTexts.Add(Text);
            }
        }
        return new FBarkRequest(ShortTexts, UID, Color, nullptr, Duration);
    }
    return new FBarkRequest(NewBarkLines, UID, Color, nullptr, Duration);
}

float FBarkRequest::GetDurationForLine(const int32 LineIndex) const
{
    if (LineIndex < 0 || LineIndex >= BarkLines.Num()) return 0.0f;
    if (TotalCharacterCount == 0)
    {
        for (const FText& Line : BarkLines)
        {
            TotalCharacterCount += Line.ToString().Len();
        }
        TotalLineCount = BarkLines.Num();
    }
    return RequestDuration * static_cast<float>(BarkLines[LineIndex].ToString().Len()) / static_cast<float>(TotalCharacterCount);
}

FBarkRequest* FBarkRequest::CreatePlayerRequest(const FText& NewBarkLine, const float Duration, const int32 UID)
{
    if (NewBarkLine.ToString().Contains(NEW_LINE_SEPARATOR))
    {
        const auto Lines = AdvGameUtils::NewLineSeperatedToArrayText(NewBarkLine);
        return CreatePlayerMultilineRequest(Lines, Duration, UID);
    }
    else
    {
        return CreatePlayerMultilineRequest({NewBarkLine}, Duration, UID);
    }
}

FBarkRequest* FBarkRequest::CreateNPCRequest(const FText& NewBarkLine, const float Duration, USphereComponent* Position,
                                             const FColor Color,
                                             const int32 UID)
{
    FBarkRequest* NPCRequest = CreatePlayerRequest(NewBarkLine, Duration, UID);
    NPCRequest->IsPlayerRequest = false;
    NPCRequest->RequestPosition = Position;
    NPCRequest->RequestColor = Color;
    return NPCRequest;
}

FBarkRequest* FBarkRequest::CreateNPCMultilineRequest(const TArray<FText>& NewBarkLines, float Duration,
                                                      USphereComponent* Position, FColor Color,
                                                      int32 UID)
{
    FBarkRequest* NPCRequest = CreatePlayerMultilineRequest(NewBarkLines, Duration, UID);
    NPCRequest->IsPlayerRequest = false;
    NPCRequest->RequestPosition = Position;
    NPCRequest->RequestColor = Color;
    return NPCRequest;
}
