// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "VerbType.h"
#include "GameFramework/PlayerController.h"
#include "AdventurePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API AAdventurePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAdventurePlayerController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	EVerbType CurrentVerb = EVerbType::None;
};
