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
			UE_LOG(LogInput, Warning, TEXT("AddToViewport"));
		}
	}

#if PLATFORM_IOS
	// camera->SetOrthoWidth(iOSCameraOrthoWidth);
#endif
}
