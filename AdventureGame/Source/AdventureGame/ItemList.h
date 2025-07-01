// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemList.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UItemList : public UObject
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EItemList: uint8
{
	None        = 0 UMETA(DisplayName = "NONE"),
	Pickle      = 1 UMETA(DisplayName = "PICKLE"),
	PickleKey   = 2 UMETA(DisplayName = "PICKLE KEY"),
	Knife       = 3 UMETA(DisplayName = "KNIFE"),
};

FString ItemGetDescriptiveString(EItemList ItemList);