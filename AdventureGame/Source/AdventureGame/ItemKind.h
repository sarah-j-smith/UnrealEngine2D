// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemKind.generated.h"

UENUM(BlueprintType)
enum class EItemKind: uint8
{
    None = 0 UMETA(DisplayName = "NONE"),
    Pickle = 1 UMETA(DisplayName = "PICKLE"),
    PickleKey = 2 UMETA(DisplayName = "PICKLE KEY"),
    Knife = 3 UMETA(DisplayName = "KNIFE"),
};
