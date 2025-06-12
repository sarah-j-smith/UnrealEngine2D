// (c) 2025 Sarah Smith


#include "AdventureGameHUD.h"
#include "AdventureCharacter.h"
#include "AdventureGame.h"
#include "Kismet/GameplayStatics.h"
#include "AdventurePlayerController.h"

void UAdventureGameHUD::NativeOnInitialized()
{
	PlayerCharacter = GetOwningPlayerPawn<AAdventureCharacter>();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
	if (!AdventurePlayerController)
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("UAdventureGameHUD::NativeOnInitialized Failed to get AAdventurePlayerController"));
	}

	UE_LOG(LogInput, Warning, TEXT("UAdventureGameHUD::NativeOnInitialized"));
}

void UAdventureGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	bool MouseIsOverUI = IsHovered();
	if (MouseIsOverUI != AdventurePlayerController->IsMouseOverUI)
	{
		AdventurePlayerController->IsMouseOverUI = MouseIsOverUI;
		FString MouseIsOverUIText = MouseIsOverUI ? "true" : "false";
		UE_LOG(LogAdventureGame, Warning, TEXT("UAdventureGameHUD::NativeTick set IsMouseOverUI %s"), *MouseIsOverUIText);
	}
}

