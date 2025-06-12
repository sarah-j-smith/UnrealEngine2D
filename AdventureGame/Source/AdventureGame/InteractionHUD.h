// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionHUD.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UInteractionHUD : public UUserWidget
{
	GENERATED_BODY()
public:

private:
	bool IsMouseOverUI();
	
};
