// (c) 2025 Sarah Smith


#include "AdventureGameModeBase.h"

#include "Kismet/GameplayStatics.h"

void AAdventureGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetupHUD();
}

void AAdventureGameModeBase::SetupHUD()
{
	if (AdventureHUDClass) {
		APlayerController *playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AdventureHUDWidget = CreateWidget<UAdventureGameHUD>(playerController, AdventureHUDClass);
		if (AdventureHUDWidget)
		{
			AdventureHUDWidget->AddToViewport();
			// PlayerHUDWidget->SetHP(hitPoints);
			// PlayerHUDWidget->SetDiamonds(MyGameInstance->DiamondCount);
			// PlayerHUDWidget->SetLevel(MyGameInstance->CurrentLevelIndex);

			// GetWorldTimerManager().SetTimer(
			// 	AttributionHideTimer, this,
			// 	&APlayerCharacter::OnAttributionHideTimerTimeout, 1.0, false, AttributionHideDelay);
		}
	}

#if PLATFORM_IOS
	// camera->SetOrthoWidth(iOSCameraOrthoWidth);
#endif
}
