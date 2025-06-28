#pragma once

#include "CoreMinimal.h"

#include "InteractionType.generated.h"

UENUM(BlueprintType)
enum class EInteractionType: uint8
{
	None        = 0 UMETA(DisplayName = "NONE"),
	Interact    = 1 UMETA(DisplayName = "INTERACT"),
	Sit         = 2 UMETA(DisplayName = "SIT"),
	Climb       = 3 UMETA(DisplayName = "CLIMB"),
};

FString InteractionGetDescriptiveString(EInteractionType InteractionType);