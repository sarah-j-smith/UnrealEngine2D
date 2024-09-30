// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

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

	AMainGameMode();

	virtual void BeginPlay() override;

	void SetScore(int newScore);
	void AddScore(int amountToAdd);
};
