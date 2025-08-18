// (c) 2025 Sarah Smith


#include "AdventurePlayerController.h"

#include "AdventureAIController.h"
#include "AdventureCharacter.h"
#include "../AdventureGame.h"
#include "../Gameplay/AdventureGameInstance.h"
#include "../HUD/AdvGameUtils.h"
#include "../Items/InventoryItem.h"
#include "../HUD/ItemSlot.h"
#include "../Items/ItemList.h"
#include "../HotSpots/Door.h"
#include "../Gameplay/AdventureSave.h"
#include "AdventureGame/Gameplay/AdvBlueprintFunctionLibrary.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AAdventurePlayerController::AAdventurePlayerController()
{
    SetShowMouseCursor(true);
    DefaultMouseCursor = EMouseCursor::Crosshairs;
    bEnableMouseOverEvents = true;

    BarkTimerDelegate.BindUObject(this, &AAdventurePlayerController::OnBarkTimerTimeOut);

    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventurePlayerController"));
}

void AAdventurePlayerController::MouseEnterHotSpot(AHotSpot* HotSpot)
{
    if (CanBrowseHotspot())
    {
        CurrentHotSpot = HotSpot;
        TriggerUpdateInteractionText();
    }
}

void AAdventurePlayerController::MouseLeaveHotSpot()
{
    if (CanBrowseHotspot() && CurrentHotSpot)
    {
        CurrentHotSpot = nullptr;
        TriggerUpdateInteractionText();
    }
}

void AAdventurePlayerController::MouseEnterInventoryItem(UItemSlot* ItemSlot)
{
    if (SourceLocked == EChoiceState::Locked && TargetLocked == EChoiceState::Locked) return;
    if (ItemSlot->HasItem)
    {
        if (SourceLocked == EChoiceState::Unlocked)
        {
            SourceItem = ItemSlot->InventoryItem;
        }
        else
        {
            TargetItem = ItemSlot->InventoryItem;
        }
        CurrentItemSlot = ItemSlot;
        TriggerUpdateInventoryText();
    }
}

void AAdventurePlayerController::MouseLeaveInventoryItem()
{
    if (SourceLocked == EChoiceState::Locked && TargetLocked == EChoiceState::Locked) return;
    if (SourceLocked == EChoiceState::Unlocked)
    {
        SourceItem = nullptr;
    }
    else
    {
        TargetItem = nullptr;
    }
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

    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
    UAdventureGameInstance* AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
    AdventureGameInstance->OnLoadRoom();

    AdventureGameInstance->Inventory->AddAdventurePlayerControllerWeakRef(this);

    TriggerUpdateInteractionText();
    APawn* Pawn = SetupPuck(PlayerCharacter);
    SetupAIController(PlayerCharacter);
}

void AAdventurePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ShouldInterruptCurrentActionOnNextTick)
    {
        InterruptCurrentAction();
        ShouldInterruptCurrentActionOnNextTick = false;
    }
    if (ShouldCompleteMovementNextTick)
    {
        HandleMovementComplete();
        ShouldCompleteMovementNextTick = false;
    }
}

void AAdventurePlayerController::HandleSaveGame(const FString& GameName)
{
    if (UAdventureGameInstance* AdventureGameInstance = Cast<UAdventureGameInstance>(GetGameInstance()))
    {
        AdventureGameInstance->SaveGame();
        bool OK = UGameplayStatics::SaveGameToSlot(AdventureGameInstance->CurrentSaveGame, GameName, 0);
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("SaveGame: %s Saved - %s"), *GameName, OK ? TEXT("true") : TEXT("false"));
    }
}

void AAdventurePlayerController::HandleLoadGame(const FString& GameName)
{
    if (UAdventureGameInstance* AdventureGameInstance = Cast<UAdventureGameInstance>(GetGameInstance()))
    {
        USaveGame *LoadedGame = UGameplayStatics::LoadGameFromSlot(GameName, 0);
        if (UAdventureSave *AdventureSave = Cast<UAdventureSave>(LoadedGame))
        {
            AdventureGameInstance->CurrentSaveGame = AdventureSave;
            AdventureGameInstance->LoadGame();
            UE_LOG(LogAdventureGame, VeryVerbose, TEXT("LoadGame: %s Loaded"), *GameName);
        }
        else
        {
            UE_LOG(LogAdventureGame, VeryVerbose, TEXT("LoadGame: %s failed, or could not be cast to UAdventureSave"), *GameName);
        }
    }
    TriggerUpdateInteractionText();
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
    AdventureHUDWidget->VerbsUI->OnVerbChanged.BindDynamic(this, &AAdventurePlayerController::AssignVerb);
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

APawn* AAdventurePlayerController::SetupPuck(AAdventureCharacter* PlayerCharacter)
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
    Puck->TouchInputDelegate.AddUObject(this, &AAdventurePlayerController::HandleTouchInput);
    return Puck;
}

void AAdventurePlayerController::SetupAIController(APawn* AttachToPawn)
{
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventurePlayerController::SetupAIController"));
    AActor* AIControllerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AAdventureAIController::StaticClass());
    AAdventureAIController* AdventureAIController = Cast<AAdventureAIController>(AIControllerActor);
    check(AdventureAIController);

    AdventureAIController->Possess(AttachToPawn);
    AdventureAIController->MoveCompletedDelegate.AddDynamic(
        this, &AAdventurePlayerController::HandleAIMovementCompleteNotify);
}

void AAdventurePlayerController::HandleTouchInput(float LocationX, float LocationY)
{
    const FString Message = FString::Printf(TEXT("TouchInput x: %f, y: %f"), LocationX, LocationY);
    GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Cyan,
                        *Message,false, FVector2D(2.0, 2.0));

    if (LockInput) return;
    if (!IsValid(PlayerCharacter)) return;

    if (IsBarking) ClearBark();
    
    if (AHotSpot* HotSpot = HotSpotTapped(LocationX, LocationY))
    {
        HandleHotSpotClicked(HotSpot);
    }
    else
    {
        FVector MouseWorldLocation, MouseWorldDirection;
        DeprojectScreenPositionToWorld(LocationX, LocationY, MouseWorldLocation, MouseWorldDirection);

        const FVector PlayerLocation = PlayerCharacter->GetCapsuleComponent()->GetComponentLocation();
        MouseWorldLocation.Z = PlayerLocation.Z;
        HandleLocationClicked(MouseWorldLocation);
    }
}

void AAdventurePlayerController::HandlePointAndClickInput()
{
#if WITH_EDITOR
    const TCHAR * LockInp = LockInput ? TEXT("true") : TEXT("false");
    const TCHAR * MouseOverUI = IsMouseOverUI ? TEXT("true") : TEXT("false");
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("HandlePointAndClickInput - LockInput: %s, IsMouseOverUI: %s"),
        LockInp, MouseOverUI);
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("*** CurrentCommand: %s - Current Verb: %s"), *UEnum::GetValueAsString(CurrentCommand),
        *VerbGetDescriptiveString(CurrentVerb).ToString());
#endif
    if (LockInput || IsMouseOverUI) return;
    if (!IsValid(PlayerCharacter)) return;
    if (!Cast<AAdventureAIController>(PlayerCharacter->Controller)) return;

    if (IsBarking) ClearBark();
    
    if (AHotSpot* HotSpot = HotSpotClicked())
    {
        SetVerbAndCommandFromHotSpot(HotSpot);
        HandleHotSpotClicked(HotSpot);
    }
    else
    {
        float LocationX, LocationY;
        if (!GetMouseClickPosition(LocationX, LocationY)) return;

        FVector MouseWorldLocation, MouseWorldDirection;
        DeprojectScreenPositionToWorld(LocationX, LocationY, MouseWorldLocation, MouseWorldDirection);

        const FVector PlayerLocation = PlayerCharacter->GetCapsuleComponent()->GetComponentLocation();
        MouseWorldLocation.Z = PlayerLocation.Z;
        HandleLocationClicked(MouseWorldLocation);
    }
}

void AAdventurePlayerController::HandleHotSpotClicked(AHotSpot* HotSpot)
{
    if (!IsValid(HotSpot)) return;
#if WITH_EDITOR
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("HandleHotSpotClicked - %s - command: %s"), *HotSpot->ShortDescription.ToString(),
        *UEnum::GetValueAsString(CurrentCommand));
#endif
    switch (CurrentCommand)
    {
    case EPlayerCommand::InstantActive:
        InterruptCurrentAction();
    case EPlayerCommand::None:
    case EPlayerCommand::Hover:
        CurrentHotSpot = HotSpot;
        SourceItem = nullptr;
        TargetItem = nullptr;
        PerformInstantAction();
        break;
    case EPlayerCommand::VerbPending:
    case EPlayerCommand::UsePending:
        CurrentCommand = EPlayerCommand::Active;
        CurrentHotSpot = HotSpot;
        ClearSourceItem();
        ClearTargetItem();
        TriggerBeginAction();
        WalkToHotSpot(HotSpot);
        break;
    case EPlayerCommand::Targeting:
        CurrentCommand = EPlayerCommand::Active;
        CurrentHotSpot = HotSpot;
        ClearTargetItem();
        TriggerBeginAction();
        WalkToHotSpot(HotSpot);
        break;
    default:
        break;
    }
}

void AAdventurePlayerController::HandleLocationClicked(const FVector& Location)
{
#if WITH_EDITOR
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("HandleLocationClicked - %s - command: %s"), *Location.ToString(),
        *UEnum::GetValueAsString(CurrentCommand));
#endif
    CurrentTargetLocation = Location;
    switch (CurrentCommand)
    {
    case EPlayerCommand::InstantActive:
        InterruptCurrentAction();
    case EPlayerCommand::None:
    case EPlayerCommand::Hover:
        ClearSourceItem();
        ClearTargetItem();
        PerformInstantAction();
        break;
    default:
        break;
    }
}

UInventoryItem* AAdventurePlayerController::ItemAddToInventory(const EItemKind& ItemToAdd)
{
    if (UItemList* Inventory = GetInventoryItemList())
    {
        if (!Inventory->Contains(ItemToAdd))
        {
            if (UInventoryItem* Item = Inventory->AddItemToInventory(ItemToAdd))
            {
                Item->SetAdventurePlayerController(this);
                return Item;
            }
        }
#if WITH_EDITOR
        else
        {
            FString DebugString = FItemKind::GetDescription(ItemToAdd).ToString();
            UE_LOG(LogAdventureGame, Warning, TEXT("Cannot create %s - already held in inventory"),
                *DebugString);
        }
#endif
    }
    return nullptr;
}

void AAdventurePlayerController::ItemRemoveFromInventory(const EItemKind& ItemToRemove)
{
    if (UItemList* Inventory = GetInventoryItemList())
    {
        Inventory->RemoveItemKindFromInventory(ItemToRemove);
        if (SourceItem && !Inventory->Contains(SourceItem->ItemKind)) ClearSourceItem();
        if (TargetItem && !Inventory->Contains(TargetItem->ItemKind)) ClearTargetItem();
    }
}

void AAdventurePlayerController::ItemsRemoveFromInventory(const TSet<EItemKind>& ItemsToRemove)
{
    if (UItemList* Inventory = GetInventoryItemList())
    {
        Inventory->RemoveItemKindsFromInventory(ItemsToRemove);
        if (SourceItem && !Inventory->Contains(SourceItem->ItemKind)) ClearSourceItem();
        if (TargetItem && !Inventory->Contains(TargetItem->ItemKind)) ClearTargetItem();
    }
}

void AAdventurePlayerController::HandleInventoryItemClicked(UItemSlot* ItemSlot)
{
    if (!ItemSlot->HasItem)
    {
        // clicking an empty inventory slot clears everything out
        InterruptCurrentAction();
        return;
    }
    CurrentItemSlot = ItemSlot;

#if WITH_EDITOR
    const FString DebugString = ItemSlot->InventoryItem->Description.ToString();
    UE_LOG(LogAdventureGame, Warning, TEXT("HandleInventoryItemClicked - %s"), *DebugString);
#endif

    if (TargetLocked == EChoiceState::Locked && SourceLocked == EChoiceState::Locked)
    {
#if WITH_EDITOR
        UE_LOG(LogAdventureGame, Warning, TEXT("Ignoring further click on %s - source and target are locked"), *DebugString);
#endif
        return;
    }
    
    // This handler is only called if `HasItem` is true
    switch (CurrentCommand)
    {
    case EPlayerCommand::None:
    case EPlayerCommand::Hover:
        SourceItem = ItemSlot->InventoryItem;
        SourceLocked = EChoiceState::Locked;
        PerformInstantAction();
        break;
    case EPlayerCommand::VerbPending:
        SourceItem = ItemSlot->InventoryItem;
        SourceLocked = EChoiceState::Locked;
        PerformItemAction();
        break;
    case EPlayerCommand::UsePending:
    case EPlayerCommand::GivePending:
        CurrentVerb = CurrentCommand == EPlayerCommand::GivePending ? EVerbType::GiveItem : EVerbType::UseItem;
        SourceItem = ItemSlot->InventoryItem;
        CurrentCommand = EPlayerCommand::Targeting;
        SourceLocked = EChoiceState::Locked;
        ClearTargetItem(); // Should be clear already
        TriggerUpdateInventoryText();
        break;
    case EPlayerCommand::Targeting:
        if (SourceItem == nullptr || SourceLocked == EChoiceState::Unlocked)
        {
            UE_LOG(LogAdventureGame, Error, TEXT("Expected SourceItem locked in combine"));
            return;
        }
        if (SourceItem->ItemKind != ItemSlot->InventoryItem->ItemKind)
        {
            TargetLocked = EChoiceState::Locked;
            CurrentCommand = EPlayerCommand::Active;
            TargetItem = ItemSlot->InventoryItem;
            PerformItemInteraction();
        }
        else
        {
            UE_LOG(LogAdventureGame, Warning, TEXT("Cannot target same kind of item"));
        }
    default:
        UE_LOG(LogAdventureGame, Warning, TEXT("Ignoring inventory click"));
    }
}

bool AAdventurePlayerController::GetMouseClickPosition(float& LocationX, float& LocationY)
{
    bool bIsPressed = GetMousePosition(LocationX, LocationY);
    if (!bIsPressed)
    {
        GetInputTouchState(ETouchIndex::Type::Touch1, LocationX, LocationY, bIsPressed);
    }
    if (!bIsPressed)
    {
        return false;
    }
    const FVector2D ThisMouseClick{LocationX, LocationY};
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
    if (!HitResult.IsValidBlockingHit()) return nullptr;
    AActor* HitActor = HitResult.GetActor();
    if (AHotSpot* HotSpot = Cast<AHotSpot>(HitActor))
    {
#if WITH_EDITOR
        FString HotSpotMessage = FString::Printf(TEXT("Got HotSpot: %s"), *HotSpot->GetName());
        GEngine->AddOnScreenDebugMessage(1, 20.0, FColor::White, HotSpotMessage,
                                         false, FVector2D(2.0, 2.0));
#endif
        return HotSpot;
    }
    return nullptr;
}

AHotSpot* AAdventurePlayerController::HotSpotTapped(float X, float Y)
{
    FHitResult HitResult;
    GetHitResultUnderFinger(ETouchIndex::Type::Touch1, ECC_Visibility, true, HitResult);
    if (!HitResult.IsValidBlockingHit()) return nullptr;
    AActor* HitActor = HitResult.GetActor();
    if (AHotSpot* HotSpot = Cast<AHotSpot>(HitActor))
    {
        FString HotSpotMessage = FString::Printf(TEXT("Got HotSpot: %s"), *HotSpot->GetName());
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

void AAdventurePlayerController::WalkToHotSpot(AHotSpot* HotSpot)
{
    const UCapsuleComponent *Capsule = PlayerCharacter->GetCapsuleComponent();
    const FVector PlayerLocation = Capsule->GetComponentLocation();

    FVector HotSpotWalkToLocation = HotSpot->WalkToPosition;
    HotSpotWalkToLocation.Z = PlayerLocation.Z;

    CurrentHotSpot == nullptr;
    float Distance = FVector::Distance(HotSpotWalkToLocation, PlayerLocation);
    if (Distance < Capsule->GetScaledCapsuleRadius())
    {
        // Character is there already, or very close to. Teleport to location and carry on.
        TeleportToLocation(HotSpotWalkToLocation);
        CurrentHotSpot = HotSpot;
        return;
    }
    WalkToLocation(HotSpotWalkToLocation);
    switch (AIStatus)
    {
    case EAIStatus::Moving:
    case EAIStatus::AlreadyThere:
        // Don't set the hotspot unless we know the player can reach it.
        CurrentHotSpot = HotSpot;
    default:
        break;
    }
}

void AAdventurePlayerController::WalkToLocation(const FVector& Location)
{
    StopAIMovement();
    if (AIStatus != EAIStatus::Idle) return;
    
#if WITH_EDITOR
    if (TeleportInsteadOfWalk)
    {
        TeleportToLocation(Location);
        return;
    }
#endif
    
    AAdventureAIController* AI = Cast<AAdventureAIController>(PlayerCharacter->GetController());
    AIStatus = EAIStatus::MakingRequest;
    switch (AI->MoveToLocation(Location, 1.0))
    {
    case EPathFollowingRequestResult::Type::Failed:
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Path following request -> failed: %f %f"), Location.X, Location.Y);
        LastPathResult = EAIMoveResult::Fail;
        break;
    case EPathFollowingRequestResult::Type::RequestSuccessful:
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Path following request -> success: %f %f"), Location.X, Location.Y);
        LastPathResult = EAIMoveResult::Moving;
        AIStatus = EAIStatus::Moving;
        break;
    case EPathFollowingRequestResult::Type::AlreadyAtGoal:
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Path following request -> already there: %f %f"), Location.X,
               Location.Y);
        break;
    }
}

/**
 * Called by AI Controller to notify that pathing is finished. Can be immediately & synchronously
 * (ie before even processing any of the switch statement in the above function)
 * if the agent/player is already at the location. Or can be after moving there. Schedules
 * `HandleMovementComplete` to be called on the next timer tick.
 */
void AAdventurePlayerController::HandleAIMovementCompleteNotify(EPathFollowingResult::Type Result)
{
    UE_LOG(LogAdventureGame, Warning, TEXT("HandleAIMovementCompleteNotify"));
    if (Result == EPathFollowingResult::Success)
    {
        if (AIStatus == EAIStatus::MakingRequest)
        {
            AIStatus = EAIStatus::AlreadyThere;
        }
        LastPathResult = EAIMoveResult::Success;
        ShouldCompleteMovementNextTick = true;
    }
    else
    {
        if (CurrentHotSpot && (Result == EPathFollowingResult::Blocked || Result == EPathFollowingResult::OffPath))
        {
            FVector HotSpotLocation = CurrentHotSpot->WalkToPoint->GetComponentLocation();
            const FString Message = FString::Printf(TEXT("Movement blocked to %s - %s - is walk to point on the nav mesh?"),
                *(CurrentHotSpot->ShortDescription.ToString()), *(HotSpotLocation.ToString()));
            GEngine->AddOnScreenDebugMessage(1, 5.0, FColor::Cyan,
                                *Message,false, FVector2D(2.0, 2.0));
        }
    }
}

void AAdventurePlayerController::TeleportToLocation(const FVector& Location)
{
    FVector Dest = Location;
    Dest.Z = PlayerCharacter->GetCapsuleComponent()->GetComponentLocation().Z;
    PlayerCharacter->TeleportToLocation(Dest);
    LastPathResult = EAIMoveResult::Success;
    AIStatus = EAIStatus::AlreadyThere;
    ShouldCompleteMovementNextTick = true;
}

void AAdventurePlayerController::SetVerbAndCommandFromHotSpot(AHotSpot* HotSpot)
{
    // If the player has not selected a verb, but has clicked on a hotspot, polymorphically
    // check with the hotspot for a default command that the player might expect, such as "Look"
    // or "Open" for a closed door, or "Use" for an open door, or "Talk to" for an NPC.
    if (CurrentCommand != EPlayerCommand::None && CurrentCommand != EPlayerCommand::Hover) return;

    // Player clicked on a hotspot without specifying a verb first.
    if (CurrentVerb != EVerbType::WalkTo)
    {
        // If CurrentCommand is None/Hover then the verb _should be_ the default
        UE_LOG(LogAdventureGame, Warning, TEXT("HotSpot %s clicked with no command but verb %s unexpectedly set!!"),
            *HotSpot->ShortDescription.ToString(), *VerbGetDescriptiveString(CurrentVerb).ToString());
    }
    CurrentVerb = HotSpot->CheckForDefaultCommand();
    if (CurrentVerb != EVerbType::WalkTo)
    {
        CurrentCommand = CurrentVerb == EVerbType::Use ? EPlayerCommand::UsePending : EPlayerCommand::VerbPending;
    }
}

void AAdventurePlayerController::StopAIMovement()
{
    AAdventureAIController* AI = Cast<AAdventureAIController>(PlayerCharacter->GetController());
    if (!IsValid(AI))
    {
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("PlayerCharacter controller expected to be AIController"));
        return;
    }
    if (AIStatus == EAIStatus::Moving)
    {
        AI->StopMovement();
        PlayerCharacter->GetMovementComponent()->StopActiveMovement();
        AIStatus = EAIStatus::Idle;
    }
}

void AAdventurePlayerController::SwapSourceAndTarget()
{
    UInventoryItem* TargetItem = this->TargetItem;
    this->TargetItem = this->SourceItem;
    this->SourceItem = TargetItem;
}

void AAdventurePlayerController::HandleMovementComplete()
{
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("HandleMovementComplete"));
    AIStatus = EAIStatus::Idle;
    if (CurrentHotSpot && (LastPathResult == EAIMoveResult::Success))
    {
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("CurrentHotSpot && (LastPathResult == EAIMoveResult::Success)"));
        PlayerCharacter->SetFacingDirection(CurrentHotSpot->FacingDirection);
        PlayerCharacter->TeleportToLocation(CurrentHotSpot->WalkToPosition);
        PerformHotSpotInteraction();
        return;
    }
    InterruptCurrentAction();
}

void AAdventurePlayerController::AssignVerb(EVerbType NewVerb)
{
    ClearBark();
    ClearSourceItem();
    ClearTargetItem();
    CurrentVerb = NewVerb;
    switch (NewVerb)
    {
    case EVerbType::Use:
        CurrentCommand = EPlayerCommand::UsePending;
        break;
    case EVerbType::Give:
        CurrentCommand = EPlayerCommand::GivePending;
        break;
    case EVerbType::WalkTo:
        break;
    default:
        CurrentCommand = EPlayerCommand::VerbPending;
    }
    TriggerUpdateInteractionText();
}

void AAdventurePlayerController::PerformItemAction()
{
#if WITH_EDITOR
    const FString DebugString = SourceItem->ShortDescription.ToString();
    UE_LOG(LogAdventureGame, Warning, TEXT("PerformItemAction %s - %s"),
           *VerbGetDescriptiveString(CurrentVerb).ToString(), *DebugString);
#endif

    if (!SourceItem)
    {
        return;
    }
    CurrentCommand = EPlayerCommand::Active;

    switch (CurrentVerb)
    {
    case EVerbType::Give:
        UInventoryItem::Execute_OnGive(SourceItem);
        break;
    case EVerbType::Open:
        UInventoryItem::Execute_OnOpen(SourceItem);
        break;
    case EVerbType::Close:
        UInventoryItem::Execute_OnClose(SourceItem);
        break;
    case EVerbType::PickUp:
        UInventoryItem::Execute_OnPickUp(SourceItem);
        break;
    case EVerbType::LookAt:
        UInventoryItem::Execute_OnLookAt(SourceItem);
        break;
    case EVerbType::TalkTo:
        UInventoryItem::Execute_OnTalkTo(SourceItem);
        break;
    case EVerbType::Use:
        UInventoryItem::Execute_OnUse(SourceItem);
        break;
    case EVerbType::Push:
        UInventoryItem::Execute_OnPush(SourceItem);
        break;
    case EVerbType::Pull:
        UInventoryItem::Execute_OnPull(SourceItem);
        break;
    case EVerbType::WalkTo:
        UInventoryItem::Execute_OnLookAt(SourceItem);
        break;
    default:
        UE_LOG(LogAdventureGame, Warning, TEXT("Unexpected verb %s in PerformItemAction"),
            *VerbGetDescriptiveString(CurrentVerb).ToString())
    }
    TriggerBeginAction();
}

void AAdventurePlayerController::PerformInstantAction()
{
#if WITH_EDITOR
    FString DebugString;
    if (SourceItem) DebugString = SourceItem->ShortDescription.ToString();
    if (CurrentHotSpot && DebugString.IsEmpty()) DebugString = CurrentHotSpot->ShortDescription.ToString();
    UE_LOG(LogAdventureGame, Warning, TEXT("PerformInstantAction %s - %s"),
           *VerbGetDescriptiveString(CurrentVerb).ToString(), *DebugString);
#endif
    CurrentCommand = EPlayerCommand::InstantActive;
    if (SourceItem)
    {
        // Clicking on something in your own inventory
        UInventoryItem::Execute_OnLookAt(SourceItem);
        TriggerUpdateInventoryText();
    }
    else if (CurrentHotSpot)
    {
        WalkToHotSpot(CurrentHotSpot);
        TriggerUpdateInteractionText();
    }
    else
    {
        WalkToLocation(CurrentTargetLocation);
        TriggerUpdateInteractionText();
    }
    TriggerBeginAction(); // Highlight and lock the text
}

void AAdventurePlayerController::PerformItemInteraction()
{
    if (!TargetItem)
    {
        return;
    }
    CurrentHotSpot = nullptr;
    SetInputLocked(true);
    CurrentCommand = EPlayerCommand::Active;
    TriggerBeginAction();

    switch (CurrentVerb)
    {
    case EVerbType::GiveItem:
        UInventoryItem::Execute_OnItemGiven(TargetItem);
        break;
    case EVerbType::UseItem:
        UInventoryItem::Execute_OnItemUsed(TargetItem);
        break;
    default:
        UE_LOG(LogAdventureGame, Warning, TEXT("Unexpected interaction verb %s for perform item interaction with %s"),
               *VerbGetDescriptiveString(CurrentVerb).ToString(), *TargetItem->ShortDescription.ToString());
    }
}

void AAdventurePlayerController::TriggerUpdateInventoryText()
{
    UpdateInventoryTextDelegate.Broadcast();
}

UItemList* AAdventurePlayerController::GetInventoryItemList()
{
    UGameInstance* GameInstance = GetGameInstance();
    UAdventureGameInstance* AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
    if (!AdventureGameInstance)
    {
        return nullptr;
    }
    return AdventureGameInstance->Inventory;
}

void AAdventurePlayerController::PerformHotSpotInteraction()
{
    UE_LOG(LogAdventureGame, Warning, TEXT("PerformHotSpotInteraction - verb %s for hotspot %s"),
           *VerbGetDescriptiveString(CurrentVerb).ToString(), *CurrentHotSpot->ShortDescription.ToString());
    // This `Execute_Verb` pattern will call C++ and Blueprint overrides.
    // The use of eg CurrentHotSpot->OnClose() does not work as BP's don't do
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
    case EVerbType::GiveItem:
        AHotSpot::Execute_OnItemGiven(CurrentHotSpot);
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
    case EVerbType::UseItem:
        AHotSpot::Execute_OnItemUsed(CurrentHotSpot);
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
    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("InterruptCurrentAction"));
    SetInputLocked(false);
    
    if (const AAdventureCharacter* Pc = PlayerCharacter)
    {
        Pc->GetMovementComponent()->StopActiveMovement();
    }
    CurrentVerb = IsMouseOverUI ? EVerbType::LookAt : EVerbType::WalkTo;
    CurrentCommand = EPlayerCommand::None;
    CurrentHotSpot = nullptr;
    CurrentItemSlot = nullptr;
    ClearSourceItem();
    ClearTargetItem();
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

void AAdventurePlayerController::PlayerBark(FText BarkText, TOptional<FColor> TextColor, float TimeToPause,
    USphereComponent *Position, int32 BarkTaskUid)
{
    if (IsBarking) ClearBark();

    if (Position == nullptr)
    {
        Position = PlayerCharacter->Sphere;
    }
    IsBarking = true;
    AdventureHUDWidget->AddBarkText(BarkText, Position, TextColor.Get(PlayerCharacter->BarkColor));

    CurrentBarkTask = BarkTaskUid;
    CurrentBarkText = BarkText;

    if (TimeToPause == 0)
    {
        TimeToPause = UAdvBlueprintFunctionLibrary::GetBarkTime(BarkText.ToString());
    }
    GetWorldTimerManager().SetTimer(TimerHandle_Bark, BarkTimerDelegate, 1.0f, false, TimeToPause);
}

void AAdventurePlayerController::OnBarkTimerTimeOut()
{
    if (IsBarking)
    {
        EndBark.Broadcast(CurrentBarkText, CurrentBarkTask, true);
        CurrentBarkTask = 0;
        CurrentBarkText = FText::GetEmpty();
        ClearBark();
    }
}

void AAdventurePlayerController::ClearBark()
{
    if (CurrentBarkTask != 0 && !CurrentBarkText.IsEmpty())
    {
        // Player or something else interrupted the current bark.
        EndBark.Broadcast(CurrentBarkText, CurrentBarkTask, false);
    }
    if (IsBarking)
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Bark);
        IsBarking = false;
    }
    AdventureHUDWidget->Bark->ClearText();
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
        UE_LOG(LogAdventureGame, VeryVerbose,
               TEXT("Current character does not distinguish front from back, in turn animation"));
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
            UE_LOG(LogAdventureGame, Warning,
                   TEXT("PlayerTurnLeft called when player already facing left. Use backward?"))
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
        UE_LOG(LogAdventureGame, VeryVerbose,
               TEXT("Current character does not distinguish front from back, in turn animation"));
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
            UE_LOG(LogAdventureGame, Warning,
                   TEXT("PlayerTurnRight called when player already facing right. Use backward?"))
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
    switch (Interaction)
    {
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
