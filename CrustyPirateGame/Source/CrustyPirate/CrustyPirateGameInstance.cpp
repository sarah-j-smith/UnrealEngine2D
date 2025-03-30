// (c) 2024 Sarah Smith


#include "CrustyPirateGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UCrustyPirateGameInstance::UpdateHitPoints(int NewHP) {
    PlayerHP = NewHP;
}


void UCrustyPirateGameInstance::AddDiamond(int amount) {
    DiamondCount += amount;
}

void UCrustyPirateGameInstance::ChangeLevel(int LevelIndex)
{
    if (LevelIndex <= 0) return;
    
    CurrentLevelIndex = LevelIndex;
    
    FString levelStr = FString::Printf(TEXT("Level_%d"), LevelIndex);
    UGameplayStatics::OpenLevel(GetWorld(), FName(levelStr));
}

void UCrustyPirateGameInstance::QuitGame(class APlayerController *player)
{
    UKismetSystemLibrary::QuitGame(GetWorld(), player, EQuitPreference::Quit, false);
}

void UCrustyPirateGameInstance::RestartGame()
{
    DiamondCount = 0;
    PlayerHP = 100;
    IsDoubleJumpUnlocked = false;
    
    ChangeLevel(1);
}

