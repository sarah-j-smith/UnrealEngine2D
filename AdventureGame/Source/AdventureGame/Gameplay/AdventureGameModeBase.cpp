// (c) 2025 Sarah Smith


#include "AdventureGameModeBase.h"

#include "../AdventureGame.h"

AAdventureGameModeBase::AAdventureGameModeBase()
{
	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Construct: AAdventureGameModeBase - Custom Game Mode class"));
}

void AAdventureGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("AAdventureGameModeBase::BeginPlay"));
}
