// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdventureGameHUD.generated.h"

class AAdventureCharacter;

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API UAdventureGameHUD : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

	// virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	AAdventureCharacter *PlayerCharacter = nullptr;
};
