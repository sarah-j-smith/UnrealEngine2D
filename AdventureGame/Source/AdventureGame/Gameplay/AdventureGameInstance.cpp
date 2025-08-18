// (c) 2025 Sarah Smith


#include "AdventureGameInstance.h"

#include "../AdventureGame.h"
#include "../Player/AdventureCharacter.h"
#include "../Player/AdventurePlayerController.h"
#include "../HUD/AdventureGameHUD.h"
#include "../HUD/AdvGameUtils.h"
#include "../HotSpots/Door.h"
#include "../Items/ItemList.h"
#include "AdventureSave.h"
#include "AdventureGame/Constants.h"

#include "GameFramework/SaveGame.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

void UAdventureGameInstance::Init()
{
	Super::Init();

	CreateInventory();
	
	if (ShouldCheckForSaveGameOnLoad && UGameplayStatics::DoesSaveGameExist(SAVE_GAME_NAME, 0))
	{
		if (USaveGame *SaveGame = UGameplayStatics::LoadGameFromSlot(SAVE_GAME_NAME, 0))
		{
			CurrentSaveGame = Cast<UAdventureSave>(SaveGame);
			LoadGame();
		}
	}
}

void UAdventureGameInstance::OnLoadRoom()
{
	UE_LOG(LogAdventureGame, Display, TEXT("UAdventureGameInstance::OnLoadRoom - RoomTransitionPhase: %s"),
		*(UEnum::GetValueAsString(RoomTransitionPhase)));
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
	// Load the room whose level name is in StartingLevelName, then call OnRoomLoaded.
	// This is done before there is a scene, or a player controller. Since there is no
	// scene we don't have to unload that previous level. 
	UE_LOG(LogAdventureGame, Log, TEXT("UAdventureGameInstance::LoadStartingRoom - %s"),
		*StartingLevelName.ToString());
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
		UE_LOG(LogAdventureGame, Log, TEXT("UAdventureGameInstance::OnRoomLoaded - LoadStartingRoom"));
		CurrentLevelName = StartingLevelName;
		CurrentDoorLabel = StartingDoorLabel;
		RoomTransitionPhase = ERoomTransitionPhase::NewRoomLoaded;
		NewRoomDelay();
		break;
	case ERoomTransitionPhase::LoadNewRoom:
		UE_LOG(LogAdventureGame, Log, TEXT("UAdventureGameInstance::OnRoomLoaded - LoadNewRoom"));
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
	auto f = UGameplayStatics::GetCurrentLevelName(GetWorld());
	UE_LOG(LogAdventureGame, Display, TEXT("UAdventureGameInstance::SetupRoom - %s"), *f);

	UE_LOG(LogAdventureGame, Display, TEXT("Look for door %s"), *CurrentDoorLabel.ToString());
	
	// Find the door with the label.
	const ADoor* Door = FindDoor(CurrentDoorLabel);
	LoadDoor(Door);
	CurrentDoor = const_cast<ADoor*>(Door);
	
	if (UAdventureGameHUD* HUD = GetHUD())
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

void UAdventureGameInstance::SaveGame()
{
	if (!IsValid(CurrentSaveGame))
	{
		CurrentSaveGame = Cast<UAdventureSave>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	}
	CurrentSaveGame->StartingLevel = CurrentDoor->CurrentLevel;
	CurrentSaveGame->StartingDoorLabel = CurrentDoor->DoorLabel;
	TArray<UInventoryItem*> Items;
	CurrentSaveGame->Inventory.Reset(Inventory->InventorySize);
	Inventory->GetInventoryItemsArray(Items);
	for (const UInventoryItem *Item : Items)
	{
		CurrentSaveGame->Inventory.Add(Item->ItemKind);
	}

	CurrentSaveGame->AdventureTags = AdventureTags;

	CurrentSaveGame->OnAdventureSave(this);
}

void UAdventureGameInstance::LoadGame()
{
	if (!IsValid(CurrentSaveGame)) return;

	if (RoomTransitionPhase == ERoomTransitionPhase::GameNotStarted)
	{
		StartingDoorLabel = CurrentSaveGame->StartingDoorLabel;
		StartingLevelName = CurrentSaveGame->StartingLevel;
		
		UE_LOG(LogAdventureGame, Display, TEXT("UAdventureGameInstance::LoadGame - GameNotStarted"));
	}
	else if (CurrentSaveGame->StartingLevel != CurrentLevelName)
	{
		SetLoadTarget(CurrentSaveGame->StartingLevel, CurrentSaveGame->StartingDoorLabel);
	}

	AAdventurePlayerController* AdventurePlayerController = GetAdventureController();
	for (const EItemKind Item : CurrentSaveGame->Inventory)
	{
		UInventoryItem *NewItem = Inventory->AddItemToInventory(Item);
		if (AdventurePlayerController)
		{
			NewItem->SetAdventurePlayerController(AdventurePlayerController);
		}
	}

	AdventureTags = CurrentSaveGame->AdventureTags;
	
	CurrentSaveGame->OnAdventureLoad(this);
}

void UAdventureGameInstance::LoadRoom()
{
	// Load the room whose level name is in CurrentLevelName, then call OnRoomLoaded.
	// This is done when there is a scene, and a player controller, we must blank the screen,
	// stop player input, and unload that previous level (that unload is done in OnRoomLoaded). 
	RoomTransitionPhase = ERoomTransitionPhase::LoadNewRoom;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController* AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	AdventurePlayerController->SetInputLocked(true);
	AdventurePlayerController->InterruptCurrentAction();
	GetHUD()->ShowBlackScreen();

	UE_LOG(LogAdventureGame, Display, TEXT("UAdventureGameInstance::LoadRoom - %s"), *CurrentLevelName.ToString());

	FLatentActionInfo LatentActionInfo = GetLatentActionForHandler(OnRoomLoadedName);
	UGameplayStatics::LoadStreamLevel(GetWorld(), CurrentLevelName,
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
#if WITH_EDITOR
		UE_LOG(LogAdventureGame, Display, TEXT("UAdventureGameInstance::FindDoor - got: %s"),
		       *(Cast<ADoor>(*FoundDoor)->ShortDescription.ToString()));
#endif
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
	
	UE_LOG(LogAdventureGame, Display, TEXT("UAdventureGameInstance::LoadDoor: %s"),
	       *(Cast<ADoor>(Door)->ShortDescription.ToString()));
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AAdventurePlayerController* AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	AAdventureCharacter* AdventureCharacter = AdventurePlayerController->PlayerCharacter;

	FVector Location = Door->WalkToPoint->GetComponentLocation();
	Location.Z = AdventureCharacter->GetCapsuleComponent()->GetComponentLocation().Z;
	AdventureCharacter->TeleportToLocation(Location);
	TArray<FName> Sockts = Door->WalkToPoint->GetAllSocketNames();
	for (FName Sockt : Sockts)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("UAdventureGameInstance::LoadDoor - got: %s"), *Sockt.ToString());
	}
	AdventureCharacter->SetFacingDirection(Door->FacingDirection);
	AdventureCharacter->SetupCamera();
}

void UAdventureGameInstance::LogSaveGameStatus(USaveGame* SaveGame)
{
}

void UAdventureGameInstance::CreateInventory()
{
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
	AdventureGameInstance->SetDestinationFromDoor(FromDoor);
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
