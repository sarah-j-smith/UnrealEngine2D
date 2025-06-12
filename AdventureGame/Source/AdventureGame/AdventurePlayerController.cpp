// (c) 2025 Sarah Smith


#include "AdventurePlayerController.h"

#include "AdventureAIController.h"
#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

AAdventurePlayerController::AAdventurePlayerController()
{
	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAdventurePlayerController::MouseEnterHotSpot(AHotSpot* HotSpot)
{
	GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, HotSpot->GetName(),
	                                 false, FVector2D(2.0, 2.0));
}

void AAdventurePlayerController::MouseLeaveHotSpot()
{
	GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, TEXT("Left hotspot"),
	                                 false, FVector2D(2.0, 2.0));
}

void AAdventurePlayerController::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = GetPawn();
	PlayerCharacter = Cast<AAdventureCharacter>(PlayerPawn);
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("%hs - PlayerCharacter not defined"), __FILE__);
		return;
	}

	AController* PlayerController = PlayerCharacter->GetController();
	AAdventureAIController* AIController = Cast<AAdventureAIController>(PlayerController);
	if (IsValid(AIController))
	{
		AIController->MoveCompletedDelegate.AddDynamic(this, &AAdventurePlayerController::HandleMovementComplete);
	}
	else
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("Expected possessing controller to be AI controller"));
	}
}

void AAdventurePlayerController::HandlePointAndClickInput()
{
	const auto HitResult = GetClicked();
	if (HitResult != ClickResult::WalkToLocation)
	{
		// Either rejected or got a hotspot
		return;
	}

	// Process WalkToLocation
	float LocationX, LocationY;
	FVector MouseWorldLocation;
	bool bIsPressed = GetMousePosition(LocationX, LocationY);
	if (!bIsPressed)
	{
		// No mouse device?  Maybe its a touch device
		GetInputTouchState(ETouchIndex::Type::Touch1, LocationX, LocationY, bIsPressed);
	}
	if (!bIsPressed) return;
	if (IsDuplicateClick(LocationX, LocationY)) return;

	UE_LOG(LogAdventureGame, Warning, TEXT("HandlePointAndClick got click: %f %f"), LocationX, LocationY);
	FVector MouseWorldDirection;
	DeprojectScreenPositionToWorld(LocationX, LocationY, MouseWorldLocation, MouseWorldDirection);

	if (IsOutsideGamePlayArea(MouseWorldLocation)) return;

	FVector PlayerLocation = PlayerCharacter->GetCapsuleComponent()->GetComponentLocation();
	MouseWorldLocation.Z = PlayerLocation.Z;
	WalkToLocation(MouseWorldLocation);
}

void AAdventurePlayerController::ClearCurrentPath()
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogAdventureGame, Warning,
		       TEXT("AAdventurePlayerController::WalkToHotpot - PlayerCharacter not defined"));
		return;
	}
	AController* PlayerController = PlayerCharacter->GetController();
	AAdventureAIController* AIController = Cast<AAdventureAIController>(PlayerController);
	AIController->StopMovement();
	InterruptCurrentAction();
}

AAdventurePlayerController::ClickResult AAdventurePlayerController::GetClicked()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (!HitResult.IsValidBlockingHit())
	{
		GetHitResultUnderFinger(ETouchIndex::Type::Touch1, ECC_Visibility, true, HitResult);
	}

	AActor* HitActor = HitResult.GetActor();
	if (AHotSpot* HotSpot = Cast<AHotSpot>(HitActor))
	{
		// Found actor clicked
		WalkToHotpot(HotSpot);
		return ClickResult::HitHotspot;
	}

	// TODO: Check the click is on the scene
	return ClickResult::WalkToLocation;

	// TODO: If not on the scene and not on a hotspot return rejected.
	// return ClickResult::Rejected;
}

void AAdventurePlayerController::WalkToHotpot(AHotSpot* HotSpot)
{
	AAdventureAIController *AIController = GetAIController();
	if (!(AIController)) return;
	auto Result = AIController->MoveToLocation(HotSpot->WalkToPosition, 2.0f);
	switch (Result)
	{
	case EPathFollowingRequestResult::Type::Failed:
		UE_LOG(LogAdventureGame, Warning, TEXT("Path following request failed: %f %f"), HotSpot->WalkToPosition.X,
		       HotSpot->WalkToPosition.Y);
	case EPathFollowingRequestResult::Type::RequestSuccessful:
		CurrentHotSpot = HotSpot;
		UE_LOG(LogAdventureGame, Warning, TEXT("Path following request success: %f %f"), HotSpot->WalkToPosition.X,
		       HotSpot->WalkToPosition.Y);
	case EPathFollowingRequestResult::Type::AlreadyAtGoal:
		CurrentHotSpot = HotSpot;
		HandleMovementComplete(EPathFollowingResult::Success);
		UE_LOG(LogAdventureGame, Warning, TEXT("Path following request already there: %f %f"),
		       HotSpot->WalkToPosition.X, HotSpot->WalkToPosition.Y);
	}
}

bool AAdventurePlayerController::IsDuplicateClick(float LocationX, float LocationY)
{
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(LastMouseClick.X, LocationX)
		&& UKismetMathLibrary::NearlyEqual_FloatFloat(LastMouseClick.Y, LocationY))
	{
		UE_LOG(LogAdventureGame, Verbose, TEXT("HandlePointAndClick ignoring duplicate mouse click: %f %f"), LocationX,
		       LocationY);
		return true;
	}
	LastMouseClick.X = LocationX;
	LastMouseClick.Y = LocationY;
	return false;
}

bool AAdventurePlayerController::IsOutsideGamePlayArea(FVector MouseWorldLocation)
{
	if (!GamePlayArea.Contains(FIntPoint(MouseWorldLocation.X, MouseWorldLocation.Y)))
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("HandlePointAndClick ignoring mouse click outside GamePlayArea: %f %f %f"),
			   MouseWorldLocation.X,
			   MouseWorldLocation.Y,
			   MouseWorldLocation.Z);
		return true;
	};
	return false;
}

void AAdventurePlayerController::WalkToLocation(FVector WorldLocation)
{
	AAdventureAIController *AIController = GetAIController();
	if (!AIController) return;
	auto Result = AIController->MoveToLocation(WorldLocation, 2.0f);
	switch (Result)
	{
	case EPathFollowingRequestResult::Type::Failed:
		UE_LOG(LogAdventureGame, Warning, TEXT("WalkToLocation request failed: %f %f"), WorldLocation.X,
			   WorldLocation.Y);
		break;
	case EPathFollowingRequestResult::Type::RequestSuccessful:
		UE_LOG(LogAdventureGame, Warning, TEXT("WalkToLocation request success: %f %f"), WorldLocation.X,
			   WorldLocation.Y);
		break;
	case EPathFollowingRequestResult::Type::AlreadyAtGoal:
		HandleMovementComplete(EPathFollowingResult::Success);
		UE_LOG(LogAdventureGame, Warning, TEXT("WalkToLocation already there: %f %f"),
			   WorldLocation.X, WorldLocation.Y);
		break;
	}
}

AAdventureAIController *AAdventurePlayerController::GetAIController()
{
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogAdventureGame, Warning,
			   TEXT("AAdventurePlayerController::WalkToHotpot - PlayerCharacter not defined"));
		return nullptr;
	}
	AController* PlayerController = PlayerCharacter->GetController();
	AAdventureAIController *AIController = Cast<AAdventureAIController>(PlayerController);
	if (!AIController)
	{
		UE_LOG(LogAdventureGame, Warning,
			   TEXT("AI Controller not defined when expected"));
		return nullptr;
	}
	return AIController;
}

void AAdventurePlayerController::HandleMovementComplete(EPathFollowingResult::Type Result)
{
	if (CurrentHotSpot && (Result == EPathFollowingResult::Success))
	{
		PlayerCharacter->SetFacingDirection(CurrentHotSpot->FacingDirection);
		PlayerCharacter->SetPosition(CurrentHotSpot->WalkToPosition);
	}
	PerformInteraction();

	InterruptCurrentAction();
}

void AAdventurePlayerController::PerformInteraction()
{
	switch (CurrentVerb)
	{
	case EVerbType::Close:
		CurrentHotSpot->OnClose_Implementation();
		break;
	case EVerbType::Open:
		CurrentHotSpot->OnOpen_Implementation();
		break;
	case EVerbType::Give:
		CurrentHotSpot->OnGive_Implementation();
		break;
	case EVerbType::LookAt:
		CurrentHotSpot->OnLookAt_Implementation();
		break;
	case EVerbType::PickUp:
		CurrentHotSpot->OnPickUp_Implementation();
		break;
	case EVerbType::TalkTo:
		CurrentHotSpot->OnTalkTo_Implementation();
		break;
	case EVerbType::Pull:
		CurrentHotSpot->OnPull_Implementation();
		break;
	case EVerbType::Push:
		CurrentHotSpot->OnPush_Implementation();
		break;
	case EVerbType::Use:
		CurrentHotSpot->OnUse_Implementation();
		break;
	default:
		break;
	}
}

void AAdventurePlayerController::InterruptCurrentAction()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetMovementComponent()->StopActiveMovement();
	}
	CurrentVerb = EVerbType::WalkTo;
	CurrentHotSpot = nullptr;
	RunInterruptedActionDelegate.ExecuteIfBound();
}
