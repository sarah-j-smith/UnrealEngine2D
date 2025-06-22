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
	check(AdventurePlayerController);
	AdventurePlayerController->BeginActionDelegate.AddUObject(this, &UAdventureGameHUD::BeginActionEvent);
	AdventurePlayerController->UpdateInteractionTextDelegate.AddUObject(this, &UAdventureGameHUD::UpdateInteractionTextEvent);
	AdventurePlayerController->InterruptActionDelegate.AddUObject(this, &UAdventureGameHUD::InterruptActionEvent);

	UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameHUD::NativeOnInitialized"));
}

void UAdventureGameHUD::ShowBlackScreen()
{
	BlackScreen->SetVisibility(ESlateVisibility::Visible);
}

void UAdventureGameHUD::HideBlackScreen()
{
	BlackScreen->SetVisibility(ESlateVisibility::Hidden);
}

void UAdventureGameHUD::SetInteractionText()
{
	auto CurrentVerb = AdventurePlayerController->CurrentVerb;
	auto CurrentHotspot = AdventurePlayerController->CurrentHotSpot;
	FString VerbStr = VerbGetDescriptiveString(CurrentVerb);
	if (CurrentHotspot)
	{
		FString HotspotStr = CurrentHotspot->HotSpotDescription;
		FString hpStr = FString::Printf(TEXT("%s %s"), *VerbStr, *HotspotStr);
		InteractionUI->SetText(hpStr);
		if (AdventurePlayerController->HotspotInteraction)
		{
			InteractionUI->HighlightText();
		}
	}
	else
	{
		InteractionUI->SetText(VerbStr);
	}
}

void UAdventureGameHUD::UpdateInteractionTextEvent()
{
	SetInteractionText();
}

void UAdventureGameHUD::BeginActionEvent()
{
	InteractionUI->HighlightText();
}

void UAdventureGameHUD::InterruptActionEvent()
{
	InteractionUI->ResetText();
	VerbsUI->ClearActiveButton();
}

void UAdventureGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	bool MouseIsOverUI = IsHovered();
	if (MouseIsOverUI != AdventurePlayerController->IsMouseOverUI)
	{
		AdventurePlayerController->IsMouseOverUI = MouseIsOverUI;
		UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UAdventureGameHUD::NativeTick set IsMouseOverUI %s"),
			*(FString(MouseIsOverUI ? "true" : "false")));
	}
}

