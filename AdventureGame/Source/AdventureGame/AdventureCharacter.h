// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventurePlayerController.h"
#include "BarkText.h"
#include "PaperZDCharacter.h"
#include "FollowCamera.h"
#include "AdventureCharacter.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API AAdventureCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	AAdventureCharacter();

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//////////////////////////////////
	///
	/// MOVEMENT
	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector TargetPlayerLocation = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Gameplay)
	FVector2D LastNonZeroMovement = FVector2D::ZeroVector;
	
	FVector2D LastVelocity = FVector2D::ZeroVector;

	void SetPosition(const FVector& NewPosition);

	void SetFacingDirection(EWalkDirection Direction);

	//////////////////////////////////
	///
	/// BARKING
	///

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Barking)
	TSubclassOf<UBarkText> BarkTextClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Barking)
	TObjectPtr<UBarkText> BarkText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Barking)
	FVector2D BarkTextSize = FVector2D(800, 180);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Barking)
	TObjectPtr<UWidgetComponent> BarkTextComponent;

	void PlayerBark(FString NewBarkText);

	void ClearBark();

	void BarkTimerTimeout();

	FTimerHandle BarkTimerHandle;

	bool bBarkTimerActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Barking)
	float BarkDelay = 3.0f;
	
	//////////////////////////////////
	///
	/// CAMERA
	///

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AFollowCamera> CameraActorToSpawn;

	void SetupCamera();
};
