// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventurePlayerController.h"
#include "BarkText.h"
#include "PaperZDCharacter.h"
#include "PaperZDAnimInstance.h"
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
	/// CUSTOM ANIMATIONS
	///

	FZDOnAnimationOverrideEndSignature OnClimbOverrideEndDelegate;

	FZDOnAnimationOverrideEndSignature OnInteractOverrideEndDelegate;
	
	/// Animation sequence for interacting with objects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperZDAnimSequence *InteractAnimationSequence;
	
	/// Animation sequence for climbing on objects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UPaperZDAnimSequence *ClimbAnimationSequence;

	void Climb();

	void Interact();

private:
	void OnInteractAnimOverrideEnd(bool completed);
	
	void OnClimbAnimOverrideEnd(bool completed);

public:
	
	//////////////////////////////////
	///
	/// MOVEMENT
	///

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Gameplay)
	FVector2D LastNonZeroMovement = FVector2D::ZeroVector;
	
	FVector2D LastVelocity = FVector2D::ZeroVector;

	void TeleportToLocation(FVector NewLocation);

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
	FVector2D BarkTextSize = FVector2D(400, 180);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Barking)
	TObjectPtr<UWidgetComponent> BarkTextComponent;

	void PlayerBark(FText NewBarkText);

	void ClearBark();

	void BarkTimerTimeout();

	void ConstrainBarkText();

	FTimerHandle BarkTimerHandle;

	bool bBarkTimerActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Barking)
	float BarkDelay = 3.0f;

private:

	/// Vector from the Capsule position to the postion of the bark text
	FVector BarkRelativeOffset;
	
	void InitializeBarkTextConfines(AFollowCamera *Camera);

	FVector BarkConfineMax;
	FVector BarkConfineMin;
	
	//////////////////////////////////
	///
	/// CAMERA
	///

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TSubclassOf<AFollowCamera> CameraActorToSpawn;

	void SetupCamera();
};
