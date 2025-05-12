// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrustyPirateGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UCrustyPirateGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int PlayerHP = 100;
	
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int DiamondCount = 0;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    bool IsDoubleJumpUnlocked = false;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    int CurrentLevelIndex = 1;
    
    void UpdateHitPoints(int NewHP);
    
    void AddDiamond(int amount);
    
    void ChangeLevel(int LevelIndex);
    
    UFUNCTION(BlueprintCallable)
    void QuitGame(class APlayerController *player);
    
    UFUNCTION(BlueprintCallable)
    void RestartGame();
};
