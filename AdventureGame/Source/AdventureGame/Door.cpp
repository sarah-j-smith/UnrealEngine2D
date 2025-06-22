// (c) 2025 Sarah Smith


#include "Door.h"

#include "AdventureGameInstance.h"

void ADoor::OnUse_Implementation()
{
	Super::OnUse_Implementation();
	UAdventureGameInstance::LoadRoom(this);
}

void ADoor::OnWalkTo_Implementation()
{
	Super::OnWalkTo_Implementation();
	UAdventureGameInstance::LoadRoom(this);
}

void ADoor::OnOpen_Implementation()
{
	Super::OnOpen_Implementation();
	UAdventureGameInstance::LoadRoom(this);
}
