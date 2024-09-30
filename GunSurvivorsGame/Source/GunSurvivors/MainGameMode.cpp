// (c) 2024 Sarah Smith


#include "MainGameMode.h"

AMainGameMode::AMainGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay()
{
    Super::BeginPlay();
    SetScore(0);
}

void AMainGameMode::SetScore(int newScore)
{
    if (newScore >= 0) {
        score = newScore;

        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Score: %d"), score));
    }
}

void AMainGameMode::AddScore(int amountToAdd)
{
    const int newScore = amountToAdd + score;
    SetScore(newScore);
}