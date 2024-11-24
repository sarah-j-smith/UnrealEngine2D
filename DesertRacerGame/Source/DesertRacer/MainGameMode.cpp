// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameMode.h"

#include "Kismet/GameplayStatics.h"

void AMainGameMode::ResetGameLevel(bool isWin)
{
    float ResetTime = LoseResetTime;
    if (isWin) {
        ResetTime = WinResetTime;
    }

    GetWorldTimerManager().SetTimer(
        ResetGameTimer, this, 
        &AMainGameMode::OnResetGameTimerTimeout, 1.0, false, ResetTime);
}

void AMainGameMode::OnResetGameTimerTimeout()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}
