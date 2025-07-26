// (c) 2025 Sarah Smith


#include "Door.h"

#include "../Gameplay/AdventureGameInstance.h"

void ADoor::OnUse_Implementation()
{
	Super::OnUse_Implementation();
	UAdventureGameInstance::LoadRoom(this);
}
