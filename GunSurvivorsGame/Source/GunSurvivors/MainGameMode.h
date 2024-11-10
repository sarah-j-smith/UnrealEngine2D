// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Engine/TimerHandle.h"

#include "MainGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreChangedDelegate, int, NewScore);

/**
 * 
 */
UCLASS()
class GUNSURVIVORS_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int score = 0;

	UPROPERTY(BlueprintAssignable)
	FScoreChangedDelegate scoreChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float gameRestartDelay = 0.4;

	FTimerHandle restartGameTimer;

	AMainGameMode();

	virtual void BeginPlay() override;

	void SetScore(int newScore);
	void AddScore(int amountToAdd);

	void RestartGame();
	void OnRestartGameTimer();
};
