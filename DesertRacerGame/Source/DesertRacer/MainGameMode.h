// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Engine/TimerHandle.h"

#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DESERTRACER_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoseResetTime = 1.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WinResetTime = 3.0;

	FTimerHandle ResetGameTimer;
	
	void ResetGameLevel(bool isWin);

	void OnResetGameTimerTimeout();
};
