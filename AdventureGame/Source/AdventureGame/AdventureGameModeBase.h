// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AdventureGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API AAdventureGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AAdventureGameModeBase();
	
	virtual void BeginPlay() override;
};
