// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventurePlayerController.h"
#include "PaperZDCharacter.h"
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

	// MARK: Gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector TargetPlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector2D LastNonZeroMovement = FVector2D::ZeroVector;
	
	FVector2D LastVelocity = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FIntRect GamePlayArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AFollowCamera> CameraActorToSpawn;

	void SetPosition(const FVector& NewPosition);

	void SetFacingDirection(EWalkDirection Direction);
	
	void SetupCamera();
};
