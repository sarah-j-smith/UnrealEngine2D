// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventurePlayerController.h"
#include "PaperZDCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "FollowCamera.h"

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

	FTimerDelegate CreateCameraDelegate;

	// MARK: Gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector TargetPlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector2D LastNonZeroMovement = FVector2D::ZeroVector;
	
	FVector2D LastVelocity = FVector2D::ZeroVector;

	// MARK: Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputAction* PointAndClickInput = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inputs)
	UInputMappingContext* InputMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FIntRect GamePlayArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AFollowCamera> CameraActorToSpawn;
	
	void HandlePointAndClick(const FInputActionValue& Value);

	void SetPosition(const FVector& NewPosition);

	void SetFacingDirection(EWalkDirection Direction);

	UFUNCTION()
	void SetupCamera();

private:
	AAdventurePlayerController *GetAdventurePlayerController() const;
};
