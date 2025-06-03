// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "AdventureCharacter.generated.h"

class AFollowCamera;
class UAdventureGameHUD;

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

	// MARK: Gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector TargetPlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputAction* PointAndClickInput = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector2D LastNonZeroMovement = FVector2D::ZeroVector;
	
	FVector2D LastVelocity = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FIntRect GamePlayArea;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFollowCamera> CameraActorToSpawn;
	
	void HandlePointAndClick(const FInputActionValue& Value);

	void SetupFollowCamera();
};
