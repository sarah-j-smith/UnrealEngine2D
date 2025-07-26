// (c) 2025 Sarah Smith


#include "InteractTask.h"

#include "../AdventureGame.h"
#include "../HUD/AdvGameUtils.h"
#include "AdventurePlayerController.h"

#include "Kismet/GameplayStatics.h"

UInteractTask::UInteractTask(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
	, WorldContextObject(nullptr)
	, Interaction(EInteractionType::None)
	, TimeDirection(EInteractTimeDirection::Forward)
{
	MyUID = AdvGameUtils::GetUUID();
}

UInteractTask* UInteractTask::DoInteractTask(const UObject* WorldContextObject, const EInteractionType Interaction, const EInteractTimeDirection TimeDirection)
{
	UInteractTask* BlueprintNode = NewObject<UInteractTask>();
	BlueprintNode->SetWorldContextObject(WorldContextObject);
	BlueprintNode->SetInteractionType(Interaction);
	BlueprintNode->SetTimeDirection(TimeDirection);

	UE_LOG(LogAdventureGame, Log, TEXT("UInteractTask created"));
	
	// Register with the game instance to avoid being garbage collected
	BlueprintNode->RegisterWithGameInstance(WorldContextObject);
	return BlueprintNode;
}

void UInteractTask::SetWorldContextObject(const UObject* WorldContextObject)
{
	this->WorldContextObject = WorldContextObject;
}

void UInteractTask::SetInteractionType(const EInteractionType InteractionType)
{
	this->Interaction = InteractionType;
}

void UInteractTask::Activate()
{
	Super::Activate();

	UE_LOG(LogAdventureGame, Log, TEXT("UInteractTask Activate - %s"), *(InteractionGetDescriptiveString(Interaction)));
	
	switch (Interaction)
	{
	case EInteractionType::Interact:
		Interact();
		break;
	case EInteractionType::Sit:
		Sit();
		break;
	case EInteractionType::Climb:
		Climb();
		break;
	case EInteractionType::TurnLeft:
		TurnLeft();
		break;
	case EInteractionType::TurnRight:
		UE_LOG(LogAdventureGame, Warning, TEXT("TURN RIGHT"));
		TurnRight();
	case EInteractionType::None:
		break;
	}
	GetAdventureController()->EndAction.AddUObject(this, &UInteractTask::InteractionCompleted);
}

AAdventurePlayerController* UInteractTask::GetAdventureController() const
{
	APlayerController *PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	AAdventurePlayerController *AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	return AdventurePlayerController;
}

void UInteractTask::Sit()
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		AdventurePlayerController->PlayerSit(MyUID, TimeDirection);
	}
}

void UInteractTask::Climb()
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		AdventurePlayerController->PlayerClimb(MyUID, TimeDirection);
	}
}

void UInteractTask::TurnLeft()
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		AdventurePlayerController->PlayerTurnLeft(MyUID, TimeDirection);
	}
}

void UInteractTask::TurnRight()
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("calling adv controller TURN RIGHT"));
		AdventurePlayerController->PlayerTurnRight(MyUID, TimeDirection);
	}
}

void UInteractTask::Interact()
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		AdventurePlayerController->PlayerInteract(MyUID, TimeDirection);
	}
}

void UInteractTask::InteractionCompleted(EInteractionType Interaction, int32 UID, bool bSuccess)
{
	if (Interaction == this->Interaction && UID == MyUID)
	{
		if (bSuccess)
		{
			TaskCompleted.Broadcast();
		}
		else
		{
			TaskInterrupted.Broadcast();
		}
		SetReadyToDestroy();
	}
	else
	{
		UE_LOG(LogAdventureGame, Verbose, TEXT("Ignoring task completion message UID: %d - Interaction: %s"),
			UID, *(InteractionGetDescriptiveString(Interaction)));
	}
}
