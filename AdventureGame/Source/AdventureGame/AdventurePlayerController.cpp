// (c) 2025 Sarah Smith


#include "AdventurePlayerController.h"

#include "AdventureAIController.h"
#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "AdventureGameInstance.h"
#include "AdvGameUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AAdventurePlayerController::AAdventurePlayerController()
{
	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	MovementCompleteTimerDelegate.BindUObject(this, &AAdventurePlayerController::HandleMovementComplete);
	
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventurePlayerController"));
}

void AAdventurePlayerController::MouseEnterHotSpot(AHotSpot* HotSpot)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::White, HotSpot->GetName(),
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

void AAdventurePlayerController::SetInputLocked(bool bLocked)
{
	LockInput = bLocked;
}

bool AAdventurePlayerController::IsInputLocked() const
{
	return LockInput;
}

void AAdventurePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("BeginPlay: AAdventurePlayerController"));

	APawn* PlayerPawn = GetPawn();
	PlayerCharacter = Cast<AAdventureCharacter>(PlayerPawn);
	check(PlayerCharacter);
	
	SetupHUD();

	UGameInstance *GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	UAdventureGameInstance *AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
	AdventureGameInstance->OnLoadRoom();
	
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
	AdventureAIController->MoveCompletedDelegate.AddDynamic(this, &AAdventurePlayerController::HandleAIMovementCompleteNotify);
}

void AAdventurePlayerController::HandlePointAndClickInput()
{
	if (IsPerformingTaskInteraction || LockInput) return;
	if (IsMouseOverUI) return;
	if (!IsValid(PlayerCharacter)) return;
	if (!Cast<AAdventureAIController>(PlayerCharacter->Controller)) return;
	
	if (AHotSpot *HotSpot = HotSpotClicked())
	{
		WalkToHotSpot(HotSpot);
		if (AIStatus == EAIStatus::Moving || AIStatus == EAIStatus::Done)
		{
			CurrentHotSpot = HotSpot;
			HotspotInteraction = true;
			TriggerUpdateInteractionText();
			TriggerBeginAction();
		}
	}
	else
	{
		float LocationX, LocationY;
		if (!GetMouseClickPosition(LocationX, LocationY)) return;
		
		FVector MouseWorldLocation, MouseWorldDirection;
		DeprojectScreenPositionToWorld(LocationX, LocationY, MouseWorldLocation, MouseWorldDirection);
		UE_LOG(LogAdventureGame, Warning, TEXT("Mouse world posiition: %s"), *(FVector2D(LocationX, LocationY).ToString()));
		
		FVector PlayerLocation = PlayerCharacter->GetCapsuleComponent()->GetComponentLocation();
		MouseWorldLocation.Z = PlayerLocation.Z;
		WalkToLocation(MouseWorldLocation);

		CurrentHotSpot = nullptr;
		HotspotInteraction = false;

		TriggerInterruptAction();
		TriggerUpdateInteractionText();
	}
}

bool AAdventurePlayerController::GetMouseClickPosition(float &LocationX, float &LocationY)
{
	bool bIsPressed = GetMousePosition(LocationX, LocationY);
	if (!bIsPressed)
	{
		GetInputTouchState(ETouchIndex::Type::Touch1, LocationX, LocationY, bIsPressed);
	}
	if (!bIsPressed) return false;
	const FVector2D ThisMouseClick { LocationX, LocationY };
	if (!AdvGameUtils::HasChangedMuch(ThisMouseClick, LastMouseClick))
	{
		// de-bouncing
		return false;
	}
	LastMouseClick = ThisMouseClick;
	return true;
}

AHotSpot* AAdventurePlayerController::HotSpotClicked()
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
		FString HotSpotMessage = FString::Printf(TEXT("Got HotSpot: %s"), *(HotSpot->GetName()));
		GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, HotSpotMessage,
										 false, FVector2D(2.0, 2.0));
		return HotSpot;
	}
	return nullptr;
}

void AAdventurePlayerController::ClearCurrentPath()
{
	AController* PlayerController = PlayerCharacter->GetController();
	if (AAdventureAIController* AIController = Cast<AAdventureAIController>(PlayerController))
	{
		AIController->StopMovement();
	}
}

void AAdventurePlayerController::WalkToHotSpot(AHotSpot *HotSpot)
{
	const FVector PlayerLocation = PlayerCharacter->GetCapsuleComponent()->GetComponentLocation();
	
	FVector HotSpotWalkToLocation = HotSpot->WalkToPosition;
	HotSpotWalkToLocation.Z = PlayerLocation.Z;

	WalkToLocation(HotSpotWalkToLocation);
	switch (AIStatus)
	{
	case EAIStatus::Moving:
	case EAIStatus::AlreadyThere:
		CurrentHotSpot = HotSpot;
		HotspotInteraction = true;
	default:
		break;
	}
}

void AAdventurePlayerController::WalkToLocation(const FVector &Location)
{
	AAdventureAIController *AI = Cast<AAdventureAIController>(PlayerCharacter->GetController());
	if (AIStatus == EAIStatus::Moving)
	{
		AI->StopMovement();
		PlayerCharacter->GetMovementComponent()->StopActiveMovement();
	}
	else if (AIStatus != EAIStatus::Idle)
	{
		return;
	}
	AIStatus = EAIStatus::MakingRequest;
	switch (AI->MoveToLocation(Location, 1.0))
	{
	case EPathFollowingRequestResult::Type::Failed:
		UE_LOG(LogAdventureGame, Log, TEXT("Path following request -> failed: %f %f"), Location.X, Location.Y);
		LastPathResult = EAIMoveResult::Fail;
		break;
	case EPathFollowingRequestResult::Type::RequestSuccessful:
		UE_LOG(LogAdventureGame, Verbose, TEXT("Path following request -> success: %f %f"), Location.X, Location.Y);
		LastPathResult = EAIMoveResult::Moving;
		AIStatus = EAIStatus::Moving;
		break;
	case EPathFollowingRequestResult::Type::AlreadyAtGoal:
		UE_LOG(LogAdventureGame, Verbose, TEXT("Path following request -> already there: %f %f"), Location.X, Location.Y);
		break;
	}
}

/**
 * Called by AI Controller to notify that pathing is finished. Can be immediately & synchronously
 * if the agent/player is already at the location; or can be after moving there. Schedules
 * `HandleMovementComplete` to be called on the next timer tick.
 */
void AAdventurePlayerController::HandleAIMovementCompleteNotify(EPathFollowingResult::Type Result)
{
	if (Result == EPathFollowingResult::Success)
	{
		if (AIStatus == EAIStatus::MakingRequest)
		{
			AIStatus = EAIStatus::AlreadyThere;
		}
		LastPathResult = EAIMoveResult::Success;
	}
	GetWorldTimerManager().SetTimerForNextTick(MovementCompleteTimerDelegate);
}

void AAdventurePlayerController::HandleMovementComplete()
{
	if (CurrentHotSpot && (LastPathResult == EAIMoveResult::Success))
	{
		PlayerCharacter->SetFacingDirection(CurrentHotSpot->FacingDirection);
		PlayerCharacter->TeleportToLocation(CurrentHotSpot->WalkToPosition);
		PerformInteraction();
	}
	AIStatus = EAIStatus::Idle;
	InterruptCurrentAction();
}

void AAdventurePlayerController::AssignVerb(EVerbType NewVerb)
{
	CurrentVerb = NewVerb;
	TriggerUpdateInteractionText();
}

void AAdventurePlayerController::PerformInteraction()
{
	// This `Execute_Verb` pattern will call C++ and Blueprint overrides.
	// The use of CurrentHotSpot->OnClose() does not work as BP's don't do
	// polymorphism and have to be dispatched in code.
	check(CurrentHotSpot);
	switch (CurrentVerb)
	{
	case EVerbType::Close:
		AHotSpot::Execute_OnClose(CurrentHotSpot);
		break;
	case EVerbType::Open:
		AHotSpot::Execute_OnOpen(CurrentHotSpot);
		break;
	case EVerbType::Give:
		AHotSpot::Execute_OnGive(CurrentHotSpot);
		break;
	case EVerbType::LookAt:
		AHotSpot::Execute_OnLookAt(CurrentHotSpot);
		break;
	case EVerbType::PickUp:
		AHotSpot::Execute_OnPickUp(CurrentHotSpot);
		break;
	case EVerbType::TalkTo:
		AHotSpot::Execute_OnTalkTo(CurrentHotSpot);
		break;
	case EVerbType::Pull:
		AHotSpot::Execute_OnPull(CurrentHotSpot);
		break;
	case EVerbType::Push:
		AHotSpot::Execute_OnPush(CurrentHotSpot);
		break;
	case EVerbType::Use:
		AHotSpot::Execute_OnUse(CurrentHotSpot);
		break;
	case EVerbType::WalkTo:
		AHotSpot::Execute_OnWalkTo(CurrentHotSpot);
	default:
		break;
	}
}

/**
 * Stop any current movement of the player character, unlock and clear the current verb
 * & active hotspot displayed. Run any action bound to the `RunInterruptedActionDelegate`.
 */
void AAdventurePlayerController::InterruptCurrentAction()
{
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->GetMovementComponent()->StopActiveMovement();
	}
	CurrentVerb = EVerbType::WalkTo;
	CurrentHotSpot = nullptr;
	HotspotInteraction = false;
	TriggerInterruptAction();
	RunInterruptedActionDelegate.ExecuteIfBound();
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

void AAdventurePlayerController::PlayerBark(FText BarkText)
{
	PlayerCharacter->PlayerBark(BarkText);
}