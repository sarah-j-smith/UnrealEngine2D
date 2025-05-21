// (c) 2024 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "TouchLeftRightInputModifier.generated.h"

/**
 * 
 */
UCLASS()
class CRUSTYPIRATE_API UTouchLeftRightInputModifier : public UInputModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Touch")
	float activationThreshold = 0.1f;
	
protected:
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
