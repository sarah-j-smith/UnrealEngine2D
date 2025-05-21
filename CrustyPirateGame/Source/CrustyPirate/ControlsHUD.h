// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ControlsHUD.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UControlsHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton *JumpButton;
    
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton *AttackButton;
};
