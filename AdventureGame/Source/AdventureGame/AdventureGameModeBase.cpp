// (c) 2025 Sarah Smith


#include "AdventureGameModeBase.h"

#include "Kismet/GameplayStatics.h"

AAdventureGameModeBase::AAdventureGameModeBase()
{
	SetupHUDTimerDelegate.BindUObject(this, &AAdventureGameModeBase::SetupHUD);
}

void AAdventureGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimerForNextTick(SetupHUDTimerDelegate);
}

void AAdventureGameModeBase::SetupHUD()
{
	if (AdventureHUDClass) {
		APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AdventureHUDWidget = CreateWidget<UAdventureGameHUD>(PlayerController, AdventureHUDClass);
		if (AdventureHUDWidget)
		{
			AdventureHUDWidget->AddToViewport();
			UE_LOG(LogInput, Warning, TEXT("AddToViewport"));
		}
	}
}
