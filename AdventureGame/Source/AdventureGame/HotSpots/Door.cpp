// (c) 2025 Sarah Smith


#include "Door.h"

#include "../Constants.h"
#include "../Gameplay/AdventureGameInstance.h"
#include "../Enums/VerbType.h"

#include "AdventureGame/Enums/AdventureGameplayTags.h"
#include "Internationalization/StringTableRegistry.h"

bool ADoor::LockDoor()
{
	if (CanLockDoorOrItem(DoorState))
	{
		DoorState = EDoorState::Locked;
		return true;
	}
	return false;
}

bool ADoor::OpenDoor()
{
	if (CanOpenDoorOrItem(DoorState))
	{
		DoorState = EDoorState::Opened;
		return true;
	}
	return false;
}

bool ADoor::CloseDoor()
{
	if (CanCloseDoorOrItem(DoorState))
	{
		DoorState = EDoorState::Closed;
		return true;
	}
	return false;
}

EVerbType ADoor::CheckForDefaultCommand() const
{
	switch (DoorState) {
	case EDoorState::Closed:
		return EVerbType::Open;
	case EDoorState::Opened:
		return EVerbType::Use;
	case EDoorState::Locked:
		return EVerbType::LookAt;
	default:
		break;
	}
	return Super::CheckForDefaultCommand();
}

FGameplayTagContainer ADoor::GetTags() const
{
	FGameplayTagContainer Tags = Super::GetTags();
	if (DoorState != EDoorState::Unknown) AdventureGameplayTags::SetDoorState(DoorState, Tags);
	return Tags;
}

void ADoor::SetTags(const FGameplayTagContainer& Tags)
{
	Super::SetTags(Tags);
	DoorState = AdventureGameplayTags::GetDoorState(Tags);
}

bool ADoor::UnlockDoor()
{
	if (CanUnlockDoorOrItem(DoorState))
	{
		DoorState = EDoorState::Closed;
		return true;
	}
	return false;
}

void ADoor::OnUse_Implementation()
{
	FText DoorNameReason;
	bool CanLoad = LevelToLoad.IsValidObjectName(DoorNameReason);
#if WITH_EDITOR
	if (!CanLoad && !LevelToLoad.ToString().IsEmpty())
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("Could not use door: %s"), *DoorNameReason.ToString());
	}
#endif
	if (DoorState == EDoorState::Opened && CanLoad)
	{
		UAdventureGameInstance::LoadRoom(this);
	}
	else
	{
		BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "UseDefaultText"));
	}
}

void ADoor::OnOpen_Implementation()
{
	switch (DoorState)
	{
	case EDoorState::Unknown:
		BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "NotOpenable"));
		break;
	case EDoorState::Closed:
		OpenDoor();
		break;
	case EDoorState::Opened:
		BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "AlreadyOpened"));
		break;
	case EDoorState::Locked:
		BarkAndEnd(LOCTABLE(ITEM_STRINGS_KEY, "Locked"));
		break;
	}
}
