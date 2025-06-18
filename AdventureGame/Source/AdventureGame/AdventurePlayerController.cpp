// (c) 2025 Sarah Smith


#include "AdventurePlayerController.h"

#include "AdventureAIController.h"
#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "AdvGameUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

AAdventurePlayerController::AAdventurePlayerController()
{
	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventurePlayerController"));
}

void AAdventurePlayerController::MouseEnterHotSpot(AHotSpot* HotSpot)
{
	GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, HotSpot->GetName(),
	                                 false, FVector2D(2.0, 2.0));
	CurrentHotSpot = HotSpot;
	TriggerUpdateInteractionText();
}

void AAdventurePlayerController::MouseLeaveHotSpot()
{
	GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, TEXT("Left hotspot"),
	                                 false, FVector2D(2.0, 2.0));
	if (!HotspotInteraction)
	{
		CurrentHotSpot = nullptr;
		TriggerUpdateInteractionText();
	}
}

void AAdventurePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("BeginPlay: AAdventurePlayerController"));

	APawn* PlayerPawn = GetPawn();
	PlayerCharacter = Cast<AAdventureCharacter>(PlayerPawn);
	check(PlayerCharacter);
	
	SetupHUD();
	TriggerUpdateInteractionText();
	APawn *Pawn = SetupPuck(PlayerCharacter);
	SetupAIController(PlayerCharacter);
}

void AAdventurePlayerController::SetupHUD()
{
	check(AdventureHUDClass);
	AdventureHUDWidget = CreateWidget<UAdventureGameHUD>(this, AdventureHUDClass);
	if (AdventureHUDWidget)
	{
		AdventureHUDWidget->AddToViewport();
		UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureGameModeBase::SetupHUD - AddToViewport"));
	}
}

APawn *AAdventurePlayerController::SetupPuck(AAdventureCharacter *PlayerCharacter)
{
	check(PlayerCharacter);
	UCapsuleComponent* CapsuleComp = PlayerCharacter->GetCapsuleComponent();
	check(CapsuleComp);
	FVector CapsuleLocation = CapsuleComp->GetComponentLocation();
	FVector SpawnLocation(CapsuleLocation.X, 0.0, 0.0);

	check(PuckClassToSpawn)
	APuck* Puck = GetWorld()->SpawnActor<APuck>(
		PuckClassToSpawn,
		SpawnLocation,
		FRotator::ZeroRotator);

	Puck->PointAndClickDelegate.AddUObject(this, &AAdventurePlayerController::HandlePointAndClickInput);
	return Puck;
}

void AAdventurePlayerController::SetupAIController(APawn *AttachToPawn)
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventurePlayerController::SetupAIController"));
	AActor *AIControllerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AAdventureAIController::StaticClass());
	AAdventureAIController *AdventureAIController = Cast<AAdventureAIController>(AIControllerActor);
	check(AdventureAIController);
	
	AdventureAIController->Possess(AttachToPawn);
	AdventureAIController->MoveCompletedDelegate.AddDynamic(this, &AAdventurePlayerController::HandleMovementComplete);
}

void AAdventurePlayerController::HandlePointAndClickInput()
{
	UE_LOG(LogAdventureGame, Verbose, TEXT("*** HandlePointAndClick"));
	
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

	const FVector2D ThisMouseClick { LocationX, LocationY };
	if (!AdvGameUtils::HasChangedMuch(ThisMouseClick, LastMouseClick))
	{
		return;
	}
	LastMouseClick = ThisMouseClick;

	UE_LOG(LogAdventureGame, Verbose, TEXT("HandlePointAndClick got location click: %f %f"), LocationX, LocationY);
	FVector MouseWorldDirection;
	DeprojectScreenPositionToWorld(LocationX, LocationY, MouseWorldLocation, MouseWorldDirection);

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
		UE_LOG(LogAdventureGame, Log, TEXT("Click not a valid blocking hit, trying touch..."));
		GetHitResultUnderFinger(ETouchIndex::Type::Touch1, ECC_Visibility, true, HitResult);
	}

	AActor* HitActor = HitResult.GetActor();
	if (AHotSpot* HotSpot = Cast<AHotSpot>(HitActor))
	{
		// Found actor clicked
		WalkToHotpot(HotSpot);

		FString HotSpotMessage = FString::Printf(TEXT("Got HotSpot: %s"), *(HotSpot->GetName()));
		GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, HotSpotMessage,
										 false, FVector2D(2.0, 2.0));
		
		return ClickResult::HitHotspot;
	}

	if (IsMouseOverUI)
	{
		return ClickResult::Rejected;
	}

	return ClickResult::WalkToLocation;
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

void AAdventurePlayerController::WalkToLocation(FVector WorldLocation)
{
	FString WalkToLocationString = FString::Printf(TEXT("WalkToLocation: %s"), *WorldLocation.ToString());
	GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, WalkToLocationString,
								 false, FVector2D(2.0, 2.0));
	
	AAdventureAIController *AIController = GetAIController();
	if (!AIController) return;
	auto Result = AIController->MoveToLocation(WorldLocation, 2.0f);
	HotspotInteraction = true;
	switch (Result)
	{
	case EPathFollowingRequestResult::Type::Failed:
		UE_LOG(LogAdventureGame, Warning, TEXT("WalkToLocation request failed: %f %f"), WorldLocation.X,
			   WorldLocation.Y);
		HotspotInteraction = false;
		break;
	case EPathFollowingRequestResult::Type::RequestSuccessful:
		UE_LOG(LogAdventureGame, Verbose, TEXT("WalkToLocation request success: %f %f"), WorldLocation.X,
			   WorldLocation.Y);
		break;
	case EPathFollowingRequestResult::Type::AlreadyAtGoal:
		HandleMovementComplete(EPathFollowingResult::Success);
		UE_LOG(LogAdventureGame, Log, TEXT("WalkToLocation already there: %f %f"),
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
		PerformInteraction();
	}

	InterruptCurrentAction();
}

void AAdventurePlayerController::AssignVerb(EVerbType NewVerb)
{
	CurrentVerb = NewVerb;
	TriggerUpdateInteractionText();
}

void AAdventurePlayerController::PerformInteraction()
{
	check(CurrentHotSpot);
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
	HotspotInteraction = false;
}

void AAdventurePlayerController::TriggerBeginAction()
{
	BeginActionDelegate.Broadcast();
}

void AAdventurePlayerController::TriggerInterruptAction()
{
	InterruptActionDelegate.Broadcast();
}

void AAdventurePlayerController::TriggerUpdateInteractionText()
{
	UpdateInteractionTextDelegate.Broadcast();
}
