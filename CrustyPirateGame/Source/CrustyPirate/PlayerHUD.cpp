// (c) 2024 Sarah Smith


#include "PlayerHUD.h"

//UPlayerHUD::UPlayerHUD(): UUserWidget() {}

void UPlayerHUD::SetHP(int hitPoints)
{
    FString hpStr = FString::Printf(TEXT("HP: %d"), hitPoints);
    HPText->SetText(FText::FromString(hpStr));
}

void UPlayerHUD::SetDiamonds(int amount)
{
    FString diamondStr = FString::Printf(TEXT("Diamonds: %d"), amount);
    DiamondsText->SetText(FText::FromString(diamondStr));
}

void UPlayerHUD::SetLevel(int level)
{
    FString levelStr = FString::Printf(TEXT("Level: %d"), level);
    LevelText->SetText(FText::FromString(levelStr));
}

