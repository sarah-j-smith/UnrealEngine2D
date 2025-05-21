// (c) 2024 Sarah Smith


#include "TouchLeftRightInputModifier.h"

FInputActionValue UTouchLeftRightInputModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
	FInputActionValue CurrentValue, float DeltaTime)
{
	FVector2D moveActionValue = CurrentValue.Get<FVector2D>();
	if (std::abs(moveActionValue.X) >= activationThreshold)
	{
		moveActionValue.X = moveActionValue.X > 0.0 ? 1.0f : -1.0f;
		CurrentValue = moveActionValue;
	}
	else
	{
		CurrentValue = FVector2D::ZeroVector;
	}
	return Super::ModifyRaw_Implementation(PlayerInput, CurrentValue, DeltaTime);
}
