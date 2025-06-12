// (c) 2025 Sarah Smith


#include "AdventureGameHUD.h"
#include "AdventureCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UAdventureGameHUD::NativeOnInitialized()
{
	PlayerCharacter = GetOwningPlayerPawn<AAdventureCharacter>();

	UE_LOG(LogInput, Warning, TEXT("UAdventureGameHUD::NativeOnInitialized"));
}
