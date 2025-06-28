// (c) 2025 Sarah Smith


#include "InteractTask.h"

#include "AdventureGame.h"
#include "AdventurePlayerController.h"
#include "AdvGameUtils.h"
#include "Kismet/GameplayStatics.h"

UInteractTask::UInteractTask(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
	, WorldContextObject(nullptr)
	, Interaction(EInteractionType::None)
{
	MyUID = AdvGameUtils::GetUUID();
}

UInteractTask* UInteractTask::DoInteractTask(const UObject* WorldContextObject, const EInteractionType Interaction)
{
	UInteractTask* BlueprintNode = NewObject<UInteractTask>();
	BlueprintNode->SetWorldContextObject(WorldContextObject);
	BlueprintNode->SetInteractionType(Interaction);
	
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
		AdventurePlayerController->PlayerSit(MyUID);
	}
}

void UInteractTask::Climb()
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		AdventurePlayerController->PlayerClimb(MyUID);
	}
}

void UInteractTask::Interact()
{
	if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController())
	{
		AdventurePlayerController->PlayerInteract(MyUID);
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
