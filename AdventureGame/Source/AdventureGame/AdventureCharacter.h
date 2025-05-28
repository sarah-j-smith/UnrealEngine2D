// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "AdventureCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API AAdventureCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	AAdventureCharacter();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector TargetPlayerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputAction* PointAndClickInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputMappingContext* InputMappingContext;

	void HandlePointAndClick(const FInputActionValue& Value);
};
