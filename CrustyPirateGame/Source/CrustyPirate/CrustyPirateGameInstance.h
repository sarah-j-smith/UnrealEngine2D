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
	
    void UpdateHitPoints(int NewHP);
};
