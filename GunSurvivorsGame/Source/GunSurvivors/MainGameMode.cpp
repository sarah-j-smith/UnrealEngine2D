// (c) 2024 Sarah Smith


#include "MainGameMode.h"

#include "Kismet/GameplayStatics.h"

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
        scoreChangedDelegate.Broadcast(score);
    }
}

void AMainGameMode::AddScore(int amountToAdd)
{
    const int newScore = amountToAdd + score;
    SetScore(newScore);
}

void AMainGameMode::RestartGame()
{
    GetWorldTimerManager().SetTimer(
        restartGameTimer, this,
        &AMainGameMode::OnRestartGameTimer, 1.0, false, gameRestartDelay);
}

void AMainGameMode::OnRestartGameTimer()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}
