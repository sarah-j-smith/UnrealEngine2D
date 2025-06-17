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
	if (AdventurePlayerController)
	{
		AdventurePlayerController->BeginActionDelegate.AddUObject(this, &UAdventureGameHUD::BeginActionEvent);
		AdventurePlayerController->UpdateInteractionTextDelegate.AddUObject(this, &UAdventureGameHUD::UpdateInteractionTextEvent);
		AdventurePlayerController->InterruptActionDelegate.AddUObject(this, &UAdventureGameHUD::InterruptActionEvent);
	}
	else
	{
		UE_LOG(LogAdventureGame, Warning, TEXT("UAdventureGameHUD::NativeOnInitialized Failed to get AAdventurePlayerController"));
	}

	UE_LOG(LogAdventureGame, Log, TEXT("UAdventureGameHUD::NativeOnInitialized"));
}

void UAdventureGameHUD::SetInteractionText()
{
	auto CurrentVerb = AdventurePlayerController->CurrentVerb;
	auto CurrentHotspot = AdventurePlayerController->CurrentHotSpot;
	if (CurrentVerb != EVerbType::None)
	{
		FString VerbStr = VerbGetDescriptiveString(CurrentVerb);
		if (CurrentHotspot)
		{
			FString HotspotStr = CurrentHotspot->HotSpotDescription;
			FString hpStr = FString::Printf(TEXT("%s %s"), *VerbStr, *HotspotStr);
			InteractionUI->SetText(hpStr);
		}
		else
		{
			InteractionUI->SetText(VerbStr);
		}
	}
	else
	{
		InteractionUI->ResetText();
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
		FString MouseIsOverUIText = MouseIsOverUI ? "true" : "false";
		UE_LOG(LogAdventureGame, Warning, TEXT("UAdventureGameHUD::NativeTick set IsMouseOverUI %s"), *MouseIsOverUIText);
	}
}

