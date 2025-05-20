// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "TouchscreenHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UTouchscreenHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton *AttackButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton *JumpButton;
};
