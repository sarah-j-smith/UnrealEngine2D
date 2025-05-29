// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventureGameHUD.h"
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
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAdventureGameHUD> AdventureHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAdventureGameHUD *AdventureHUDWidget;

	virtual void BeginPlay() override;
	
	void SetupHUD();
};
