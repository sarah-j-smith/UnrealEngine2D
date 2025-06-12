// (c) 2025 Sarah Smith


#include "AdventureGameModeBase.h"

#include "AdventureAIController.h"
#include "AdventurePlayerController.h"
#include "AdventureCharacter.h"

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
	AAdventurePlayerController *AdventurePlayerController = nullptr;
	if (AdventureHUDClass) {
		APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AdventureHUDWidget = CreateWidget<UAdventureGameHUD>(PlayerController, AdventureHUDClass);
		if (AdventureHUDWidget)
		{
			AdventureHUDWidget->AddToViewport();
			UE_LOG(LogInput, Warning, TEXT("AddToViewport"));
		}
		AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
		AdventurePlayerController->HUD = AdventureHUDWidget;
	}
	AActor *AIControllerActor = UGameplayStatics::GetActorOfClass(GetWorld(), AAdventureAIController::StaticClass());
	AAdventureAIController *AdventureAIController = Cast<AAdventureAIController>(AIControllerActor);
	if (IsValid(AdventureAIController))
	{
		APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		AAdventureCharacter *PlayerCharacter = Cast<AAdventureCharacter>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			AdventurePlayerController->PlayerCharacter = PlayerCharacter;
		}
		AdventureAIController->Possess(PlayerCharacter);
	}
}
