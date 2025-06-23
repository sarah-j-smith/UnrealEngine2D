// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "HotSpot.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API ADoor : public AHotSpot
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	FName LevelToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	FName CurrentLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	FName DoorLabel;

	void OnUse_Implementation() override;
};
