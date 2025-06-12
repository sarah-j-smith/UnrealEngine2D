// (c) 2025 Sarah Smith

#pragma once

#include "CoreMinimal.h"
#include "AdventureAIController.h"
#include "AdventureGameHUD.h"
#include "HotSpot.h"
#include "VerbType.h"
#include "GameFramework/PlayerController.h"
#include "AdventurePlayerController.generated.h"

DECLARE_DELEGATE(FRunInterruptedActionDelegate);

/**
 * 
 */
UCLASS()
class ADVENTUREGAME_API AAdventurePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	enum class ClickResult
	{
		HitHotspot,
		WalkToLocation,
		Rejected,
	};
	AAdventurePlayerController();

	void MouseEnterHotSpot(AHotSpot *HotSpot);

	void MouseLeaveHotSpot();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	EVerbType CurrentVerb = EVerbType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UAdventureGameHUD *HUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AAdventureCharacter *PlayerCharacter;

	FVector2D LastMouseClick = FVector2D(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FIntRect GamePlayArea;
	
	void HandlePointAndClickInput();

	UFUNCTION()
	void HandleMovementComplete(EPathFollowingResult::Type Result);

	void PerformInteraction();

	void InterruptCurrentAction();

	FRunInterruptedActionDelegate RunInterruptedActionDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TObjectPtr<AHotSpot> CurrentHotSpot;

	void ClearCurrentPath();

	
	
private:
	 ClickResult GetClicked();

	void WalkToHotpot(AHotSpot *HotSpot);

	bool IsDuplicateClick(float LocationX, float LocationY);

	bool IsOutsideGamePlayArea(FVector MouseWorldLocation);

	void WalkToLocation(FVector WorldLocation);

	AAdventureAIController *GetAIController();
};
