// (c) 2025 Sarah Smith


#include "AdventureGameInstance.h"

#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "AdventurePlayerController.h"
#include "Door.h"
#include "AdventureGameHUD.h"
#include "AdvGameUtils.h"
#include "ItemData.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAdventureGameInstance::OnLoadRoom()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameInstance::OnLoadRoom"));
	if (RoomTransitionPhase == ERoomTransitionPhase::GameNotStarted)
	{
		LoadStartingRoom();
	}
	else if (RoomTransitionPhase == ERoomTransitionPhase::RoomCurrent)
	{
		LoadRoom();
	}
}

void UAdventureGameInstance::LoadStartingRoom()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameInstance::LoadStartingRoom"));
	RoomTransitionPhase = ERoomTransitionPhase::LoadStartingRoom;
	FLatentActionInfo LatentActionInfo = GetLatentActionForHandler(OnRoomLoadedName);
	UGameplayStatics::LoadStreamLevel(GetWorld(), StartingLevelName,
	                                  true, false, LatentActionInfo);
}

void UAdventureGameInstance::OnRoomLoaded()
{
	switch (RoomTransitionPhase)
	{
	case ERoomTransitionPhase::LoadStartingRoom:
		UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameInstance::OnRoomLoaded - LoadStartingRoom"));
		CurrentDoorLabel = StartingDoorLabel;
		RoomTransitionPhase = ERoomTransitionPhase::NewRoomLoaded;
		NewRoomDelay();
		break;
	case ERoomTransitionPhase::LoadNewRoom:
		UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameInstance::OnRoomLoaded - LoadNewRoom"));
		RoomTransitionPhase = ERoomTransitionPhase::NewRoomLoaded;
		UnloadRoom();
	default:
		UE_LOG(LogAdventureGame, Warning, TEXT("Unexpected state during OnRoomLoaded"));
		break;
	}
}

void UAdventureGameInstance::NewRoomDelay()
{
	RoomTransitionPhase = ERoomTransitionPhase::DelayProcessing;

	AAdventurePlayerController* AdventurePlayerController = GetAdventureController();
	AdventurePlayerController->GetWorldTimerManager().SetTimer(
		RoomLoadTimer, this,
		&UAdventureGameInstance::OnRoomLoadTimerTimeout, 1.0, false, RoomLoadDelay);
}

void UAdventureGameInstance::OnRoomLoadTimerTimeout()
{
	SetupRoom();
}

void UAdventureGameInstance::SetupRoom()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameInstance::SetupRoom"));

	// Find the door with the label.
	const ADoor* Door = FindDoor(CurrentDoorLabel);
	LoadDoor(Door);
	
	if (UAdventureGameHUD* HUD = UAdventureGameInstance::GetHUD())
	{
		HUD->HideBlackScreen();
		if (Inventory)
		{
			if (UAdventureGameHUD *Hud = GetHUD())
			{
				UE_LOG(LogAdventureGame, Verbose, TEXT("Added handler for inventory changed"));
				OnInventoryChangedHandle = Inventory->OnInventoryChanged.AddUObject(
					Hud, &UAdventureGameHUD::HandleInventoryChanged);
			}
		}
	}

	AAdventurePlayerController* AdventurePlayerController = GetAdventureController();
	AdventurePlayerController->InterruptCurrentAction();
}

void UAdventureGameInstance::StartNewRoom()
{
	AAdventurePlayerController* AdventurePlayerController = GetAdventureController();
	AdventurePlayerController->SetInputLocked(false);
	RoomTransitionPhase = ERoomTransitionPhase::RoomCurrent;
}

void UAdventureGameInstance::OnRoomUnloaded()
{
	NewRoomDelay();
}

void UAdventureGameInstance::TriggerRoomTransition()
{
	RoomTransitionPhase = ERoomTransitionPhase::RoomCurrent;
	OnLoadRoom();
}

void UAdventureGameInstance::LoadRoom()
{
	RoomTransitionPhase = ERoomTransitionPhase::LoadNewRoom;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController* AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	AdventurePlayerController->SetInputLocked(true);
	AdventurePlayerController->InterruptCurrentAction();
	GetHUD()->ShowBlackScreen();

	// Save the CurrentDoorLabel because the CurrentDoor will be destroyed when the level is unloaded
	CurrentDoorLabel = CurrentDoor->DoorLabel;

	FLatentActionInfo LatentActionInfo = GetLatentActionForHandler(OnRoomLoadedName);
	UGameplayStatics::LoadStreamLevel(GetWorld(), CurrentDoor->LevelToLoad,
	                                  true, false, LatentActionInfo);
}

void UAdventureGameInstance::UnloadRoom()
{
	if (Inventory)
	{
		if (UAdventureGameHUD *Hud = GetHUD())
		{
			Inventory->OnInventoryChanged.Remove(OnInventoryChangedHandle);
		}
	}
	RoomTransitionPhase = ERoomTransitionPhase::UnloadOldRoom;
	FLatentActionInfo LatentActionInfo = GetLatentActionForHandler(OnRoomUnloadedName);
	UGameplayStatics::UnloadStreamLevel(GetWorld(), CurrentDoor->CurrentLevel, LatentActionInfo, false);
}

FLatentActionInfo UAdventureGameInstance::GetLatentActionForHandler(FName EventName)
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.Linkage = 0;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = EventName;
	LatentActionInfo.UUID = AdvGameUtils::GetUUID();
	return LatentActionInfo;
}

ADoor* UAdventureGameInstance::FindDoor(FName DoorLabel)
{
	TArray<AActor*> Doors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), Doors);
	if (AActor** FoundDoor = Doors.FindByPredicate([DoorLabel](auto Element)
	{
		return Cast<ADoor>(Element)->DoorLabel == DoorLabel;
	}))
	{
		UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameInstance::FindDoor - got: %s"),
		       *(Cast<ADoor>(*FoundDoor)->HotSpotDescription));
		return Cast<ADoor>(*FoundDoor);
	}
	UE_LOG(LogAdventureGame, Error, TEXT("UAdventureGameInstance::FindDoor failed to find %s"),
	       *(DoorLabel.ToString()));
	return nullptr;
}

void UAdventureGameInstance::LoadDoor(const ADoor* Door)
{
	if (!Door)
	{
		return;
	}
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameInstance::LoadDoor: %s"),
	       *(Cast<ADoor>(Door)->HotSpotDescription));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController* AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	AAdventureCharacter* AdventureCharacter = AdventurePlayerController->PlayerCharacter;

	AdventureCharacter->TeleportToLocation(Door->WalkToPosition);
	AdventureCharacter->SetFacingDirection(Door->FacingDirection);
	AdventureCharacter->SetupCamera();
}

void UAdventureGameInstance::Init()
{
	Super::Init();

	if (!Inventory)
	{
		if (InventoryClass)
		{
			Inventory = NewObject<UItemList>(this, InventoryClass, TEXT("Inventory"));
		}
		else
		{
			Inventory = NewObject<UItemList>(this, TEXT("Inventory"));
			UE_LOG(LogAdventureGame, Log, TEXT("Created new inventory of UItemList type. Set InventoryClass property in AdventureGameInstance to customise this."));
		}
	}
}

void UAdventureGameInstance::LoadRoom(ADoor* FromDoor)
{
	UWorld* World = FromDoor->GetWorld();
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(World);
	UAdventureGameInstance* AdventureGameInstance = Cast<UAdventureGameInstance>(GameInstance);
	AdventureGameInstance->CurrentDoor = FromDoor;
	AdventureGameInstance->CurrentDoorLabel = FromDoor->DoorLabel;
	AdventureGameInstance->TriggerRoomTransition();
}

UAdventureGameHUD* UAdventureGameInstance::GetHUD()
{
	if (GEngine->GameViewport)
	{
		if (UWorld* World = GEngine->GameViewport->GetWorld())
		{
			TArray<UUserWidget*> FoundWidgets;
			UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets, UAdventureGameHUD::StaticClass(), true);
			return FoundWidgets.IsEmpty() ? nullptr : Cast<UAdventureGameHUD>(FoundWidgets[0]);
		}
	}
	return nullptr;
}
