// (c) 2025 Sarah Smith


#include "AdventurePlayerController.h"

#include "AdventureAIController.h"
#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "AdventureGameInstance.h"
#include "AdvGameUtils.h"
#include "InventoryItem.h"
#include "ItemSlot.h"
#include "ItemKind.h"
#include "ItemList.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AAdventurePlayerController::AAdventurePlayerController()
{
	SetShowMouseCursor(true);
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableMouseOverEvents = true;

	MovementCompleteTimerDelegate.BindUObject(this, &AAdventurePlayerController::HandleMovementComplete);
	
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventurePlayerController"));
}

void AAdventurePlayerController::MouseEnterHotSpot(AHotSpot* HotSpot)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0, FColor::White, HotSpot->GetName(),
	                                 false, FVector2D(2.0, 2.0));
	if (!HotspotInteraction)
	{
		CurrentHotSpot = HotSpot;
		TriggerUpdateInteractionText();
	}
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

void AAdventurePlayerController::MouseEnterInventoryItem(UItemSlot *ItemSlot)
{
	if (CurrentCommand != EPlayerCommand::None) return;
	CurrentItem = ItemSlot->InventoryItem;
	CurrentItemSlot = ItemSlot;
	TriggerUpdateInventoryText();
}

void AAdventurePlayerController::MouseLeaveInventoryItem()
{
	if (CurrentCommand != EPlayerCommand::None) return;
	if (!CurrentItem) return;
	CurrentItem = nullptr;
	CurrentItemSlot = nullptr;
	TriggerUpdateInventoryText();
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
	SetupAnimationDelegates();
	
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

void AAdventurePlayerController::SetupAnimationDelegates()
{
	PlayerCharacter->AnimationCompleteDelegate.AddDynamic(this, &AAdventurePlayerController::OnPlayerAnimationComplete);
}

void AAdventurePlayerController::UpdateMouseOverUI(bool NewMouseIsOverUI)
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventurePlayerController set IsMouseOverUI %s"),
	   *(FString(NewMouseIsOverUI ? "true" : "false")));
	this->IsMouseOverUI = NewMouseIsOverUI;
	if (NewMouseIsOverUI)
	{
		if (CurrentVerb == EVerbType::WalkTo && CurrentCommand == EPlayerCommand::None)
		{
			CurrentVerb = EVerbType::LookAt;
			TriggerUpdateInteractionText();
		}
	}
	else
	{
		if (CurrentVerb == EVerbType::LookAt && CurrentCommand == EPlayerCommand::None)
		{
			CurrentVerb = EVerbType::WalkTo;
			TriggerUpdateInteractionText();
		}
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

UInventoryItem *AAdventurePlayerController::ItemAddToInventory(const EItemKind &ItemToAdd, FText Description)
{
	if (UItemList *Inventory = GetInventoryItemList())
	{
		if (!Inventory->Contains(ItemToAdd))
		{
			return Inventory->AddItemToInventory(ItemToAdd, Description);
		}
	}
	return nullptr;
}

void AAdventurePlayerController::ItemRemoveFromInventory(const EItemKind &ItemToRemove)
{
	if (UItemList *Inventory = GetInventoryItemList())
	{
		Inventory->RemoveItemKindFromInventory(ItemToRemove);
	}
}

void AAdventurePlayerController::HandleInventoryItemClicked(UItemSlot* ItemSlot)
{
	FString DebugString = ItemSlot->InventoryItem ? ItemSlot->InventoryItem->GetDescription().ToString() : "Empty Slot";
	UE_LOG(LogAdventureGame, Warning, TEXT("HandleInventoryItemClicked - %s"), *DebugString);
	if (CurrentItemSlot && ItemInteraction)
	{
		PerformItemInteraction(ItemSlot->InventoryItem);
		return;
	}
	CurrentCommand = EPlayerCommand::VerbInProgress;
	CurrentItemSlot = ItemSlot;
	ItemInteraction = true;
	CurrentItem = ItemSlot->InventoryItem;
	UInventoryItem *Item = const_cast<UInventoryItem *>(ItemSlot->InventoryItem);
	switch (CurrentVerb)
	{
	case EVerbType::Give:
		UE_LOG(LogAdventureGame, Warning, TEXT("EVerbType::Give - %s"), *DebugString);
		UInventoryItem::Execute_OnGive(Item);
		break;
	case EVerbType::Open:
		UInventoryItem::Execute_OnOpen(Item);
		break;
	case EVerbType::Close:
		UInventoryItem::Execute_OnClose(Item);
		break;
	case EVerbType::PickUp:
		UInventoryItem::Execute_OnPickUp(Item);
		break;
	case EVerbType::LookAt:
		UInventoryItem::Execute_OnLookAt(Item);
		break;
	case EVerbType::TalkTo:
		UE_LOG(LogAdventureGame, Warning, TEXT("EVerbType::TalkTo - %s"), *DebugString);
		UInventoryItem::Execute_OnTalkTo(Item);
		break;
	case EVerbType::Use:
		UInventoryItem::Execute_OnUse(Item);
		break;
	case EVerbType::Push:
		UInventoryItem::Execute_OnPush(Item);
		break;
	case EVerbType::Pull:
		UInventoryItem::Execute_OnPull(Item);
		break;
	case EVerbType::WalkTo:
		UInventoryItem::Execute_OnLookAt(Item);
		break;
	case EVerbType::UseItem:
		UInventoryItem::Execute_OnItemUsed(Item);
		break;
	case EVerbType::GiveItem:
		UInventoryItem::Execute_OnItemGiven(Item);
		break;
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

void AAdventurePlayerController::EndTaskAction(EInteractionType InteractionType, int32 UID, bool Complete)
{
	UE_LOG(LogAdventureGame, Warning, TEXT("AAdventurePlayerController::EndTaskAction - %s"),
		*(InteractionGetDescriptiveString(InteractionType)));
	if (InteractionType != EInteractionType::None)
	{
		EndAction.Broadcast(InteractionType, UID, Complete);
	}
	PlayerInteractUID = 0;
	SetInputLocked(false);
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
	CurrentCommand = EPlayerCommand::VerbActivated;
	CurrentVerb = NewVerb;
	TriggerUpdateInteractionText();
}

void AAdventurePlayerController::PerformItemInteraction(const UInventoryItem *InventoryItem)
{
	check(InventoryItem);
	switch (CurrentVerb)
	{
	case EVerbType::Give:
		UInventoryItem::Execute_OnItemGiven(const_cast<UInventoryItem *>(InventoryItem));
		break;
	case EVerbType::UseItem:
		UInventoryItem::Execute_OnItemUsed(const_cast<UInventoryItem *>(InventoryItem));
		break;
	default:
		UE_LOG(LogAdventureGame, Warning, TEXT("Unexpected interaction verb %s for perform item interaction with %s"),
			*VerbGetDescriptiveString(CurrentVerb), *(InventoryItem->GetDescription().ToString()));
	}
}

void AAdventurePlayerController::CombineItems(const UInventoryItem* InventoryItemSource,
	const UInventoryItem* InventoryItemToCombineWith, EItemKind ResultingItem, FText TextToBark, FText ResultDescription)
{
	check(InventoryItemSource);
	check(InventoryItemToCombineWith);
	check(InventoryItemToCombineWith != InventoryItemSource);
	check(InventoryItemToCombineWith->ItemKind != InventoryItemSource->ItemKind);
	check(InventoryItemSource->InteractableItem == InventoryItemToCombineWith->ItemKind)
	UGameInstance *GameInstance = GetGameInstance();
	UAdventureGameInstance *AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
	AdventureGameInstance->Inventory->RemoveItemKindsFromInventory({
		InventoryItemSource->ItemKind,
		InventoryItemToCombineWith->ItemKind
	});
	UInventoryItem *NewItem = AdventureGameInstance->Inventory->AddItemToInventory(ResultingItem, ResultDescription);
	NewItem->AdventurePlayerController = this;
	if (CurrentItem == InventoryItemSource)
	{
		CurrentItem = nullptr;
	}
	if (CurrentItemSlot && CurrentItemSlot->InventoryItem == InventoryItemToCombineWith)
	{
		CurrentItemSlot = nullptr;
	}
	PlayerBark(TextToBark);
	InterruptCurrentAction();
}

void AAdventurePlayerController::TriggerUpdateInventoryText()
{
	UpdateInventoryTextDelegate.Broadcast();
}

UItemList* AAdventurePlayerController::GetInventoryItemList()
{
	UGameInstance *GameInstance = GetGameInstance();
	UAdventureGameInstance *AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
	if (!AdventureGameInstance) return nullptr;
	return AdventureGameInstance->Inventory;
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
	if (const AAdventureCharacter *Pc = PlayerCharacter)
	{
		Pc->GetMovementComponent()->StopActiveMovement();
	}
	CurrentVerb = IsMouseOverUI ? EVerbType::LookAt : EVerbType::WalkTo;
	CurrentCommand = EPlayerCommand::None;
	CurrentHotSpot = nullptr;
	HotspotInteraction = false;
	ActiveItem = EItemKind::None;
	IsGivingItem = false;
	IsUsingItem = false;
	ItemInteraction = false;
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

void AAdventurePlayerController::PlayerClimb(int32 UID, EInteractTimeDirection InteractDirection)
{
	PlayerClimbUID = UID;
	if (PlayerCharacter->LastNonZeroMovement.X != 0)
	{
		SetInputLocked(true);
		PlayerCharacter->Climb();
	}
	else
	{
		OnPlayerClimbComplete(false);
		UE_LOG(LogAdventureGame, Warning, TEXT("PlayerClimb called when player not facing left or right."))
	}
}

void AAdventurePlayerController::PlayerInteract(int32 UID, EInteractTimeDirection InteractDirection)
{
	PlayerInteractUID = UID;
	if (PlayerCharacter->LastNonZeroMovement.X != 0)
	{
		SetInputLocked(true);
		PlayerCharacter->Interact();
	}
	else
	{
		OnPlayerInteractComplete(false);
		UE_LOG(LogAdventureGame, Warning, TEXT("PlayerInteract called when player not facing left or right."))
	}
}

void AAdventurePlayerController::PlayerSit(int32 UID, EInteractTimeDirection InteractDirection)
{
	UE_LOG(LogAdventureGame, Warning, TEXT("AAdventurePlayerController::PlayerSit"));
	PlayerSitUID = UID;
	if (PlayerCharacter->LastNonZeroMovement.X != 0)
	{
		SetInputLocked(true);
		UE_LOG(LogAdventureGame, Warning, TEXT("     >>> PlayerSit"));
		PlayerCharacter->Sit();
	}
	else
	{
		OnPlayerSitComplete(false);
		UE_LOG(LogAdventureGame, Warning, TEXT("PlayerSit called when player not facing left or right."))
	}
}

void AAdventurePlayerController::PlayerTurnLeft(int32 UID, EInteractTimeDirection InteractDirection)
{
	PlayerTurnUID = UID;
	EWalkDirection Facing = PlayerCharacter->GetFacingDirection();
	switch (Facing)
	{
	case EWalkDirection::Up:
		UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Current character does not distinguish front from back, in turn animation"));
	case EWalkDirection::Down:
		if (InteractDirection == EInteractTimeDirection::Forward)
		{
			// Turn from the front/back to the left
			SetInputLocked(true);
			PlayerCharacter->TurnLeft(InteractDirection);
		}
		else
		{
			UE_LOG(LogAdventureGame, Warning, TEXT("PlayerTurnLeft in Reverse called when player not facing left"))
			OnPlayerTurnLeftComplete(false);
		}
		break;
	case EWalkDirection::Left:
		if (InteractDirection == EInteractTimeDirection::Backward)
		{
			// Turn from the left back to the front
			SetInputLocked(true);
			PlayerCharacter->TurnLeft(InteractDirection);
		}
		else
		{
			UE_LOG(LogAdventureGame, Warning, TEXT("PlayerTurnLeft called when player already facing left. Use backward?"))
			OnPlayerTurnLeftComplete(false);
		}
		break;
	case EWalkDirection::Right:
		UE_LOG(LogAdventureGame, Warning, TEXT("PlayerTurnLeft called when player facing right."))
		OnPlayerTurnLeftComplete(false);
		break;
	}
}

void AAdventurePlayerController::PlayerTurnRight(int32 UID, EInteractTimeDirection InteractDirection)
{
	UE_LOG(LogAdventureGame, Warning, TEXT("AAdventurePlayerController::PlayerTurnRight"));

	PlayerTurnUID = UID;
	EWalkDirection Facing = PlayerCharacter->GetFacingDirection();
	switch (Facing)
	{
	case EWalkDirection::Up:
		UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Current character does not distinguish front from back, in turn animation"));
	case EWalkDirection::Down:
		if (InteractDirection == EInteractTimeDirection::Forward)
		{
			// Turn from the front/back to the left
			SetInputLocked(true);
			PlayerCharacter->TurnRight(InteractDirection);
		}
		else
		{
			UE_LOG(LogAdventureGame, Warning, TEXT("PlayerTurnRight in Reverse called when player not facing left"))
			OnPlayerTurnRightComplete(false);
		}
		break;
	case EWalkDirection::Right:
		if (InteractDirection == EInteractTimeDirection::Backward)
		{
			// Turn from the left back to the front
			SetInputLocked(true);
			PlayerCharacter->TurnRight(InteractDirection);
		}
		else
		{
			UE_LOG(LogAdventureGame, Warning, TEXT("PlayerTurnRight called when player already facing right. Use backward?"))
			OnPlayerTurnRightComplete(false);
		}
		break;
	case EWalkDirection::Left:
		UE_LOG(LogAdventureGame, Warning, TEXT("PlayerTurnRight called when player facing left."))
		OnPlayerTurnRightComplete(false);
		break;
	}
}

void AAdventurePlayerController::OnPlayerAnimationComplete(EInteractionType Interaction, bool Complete)
{
	switch (Interaction) {
	case EInteractionType::None:
		EndTaskAction(Interaction, 0, true);
		break;
	case EInteractionType::Interact:
		OnPlayerInteractComplete(Complete);
		break;
	case EInteractionType::Sit:
		OnPlayerSitComplete(Complete);
		break;
	case EInteractionType::Climb:
		OnPlayerClimbComplete(Complete);
		break;
	case EInteractionType::TurnLeft:
		OnPlayerTurnLeftComplete(Complete);
		break;
	case EInteractionType::TurnRight:
		OnPlayerTurnRightComplete(Complete);
		break;
	}
}

void AAdventurePlayerController::OnPlayerInteractComplete(bool Complete)
{
	EndTaskAction(EInteractionType::Interact, PlayerInteractUID, Complete);
}

void AAdventurePlayerController::OnPlayerSitComplete(bool Complete)
{
	EndTaskAction(EInteractionType::Sit, PlayerSitUID, Complete);
}

void AAdventurePlayerController::OnPlayerClimbComplete(bool Complete)
{
	EndTaskAction(EInteractionType::Climb, PlayerClimbUID, Complete);
}

void AAdventurePlayerController::OnPlayerTurnLeftComplete(bool Complete)
{
	EndTaskAction(EInteractionType::TurnLeft, PlayerTurnUID, Complete);
}

void AAdventurePlayerController::OnPlayerTurnRightComplete(bool Complete)
{
	EndTaskAction(EInteractionType::TurnRight, PlayerTurnUID, Complete);
}
