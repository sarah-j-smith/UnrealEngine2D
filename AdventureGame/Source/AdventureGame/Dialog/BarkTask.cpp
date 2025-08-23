// (c) 2025 Sarah Smith


#include "BarkTask.h"

#include "../AdventureGame.h"
#include "../HUD/AdvGameUtils.h"
#include "../Player/AdventurePlayerController.h"

#include "Kismet/GameplayStatics.h"

UBarkTask::UBarkTask(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , WorldContextObject(nullptr)
    , BarkText(FText::GetEmpty())
{
    MyUID = AdvGameUtils::GetUUID();
}

UBarkTask* UBarkTask::DoBarkTask(const UObject* WorldContextObject, const FText BarkText)
{
    UBarkTask* BlueprintNode = NewObject<UBarkTask>();
    BlueprintNode->WorldContextObject = WorldContextObject;

    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UBarkTask created"));

    // Register with the game instance to avoid being garbage collected
    BlueprintNode->RegisterWithGameInstance(WorldContextObject);
    return BlueprintNode;
}

void UBarkTask::Activate()
{
    Super::Activate();

    UE_LOG(LogAdventureGame, VeryVerbose, TEXT("UBarkTask Activate - %s"), *(BarkText.ToString()));

    auto Apc = GetAdventureController();
    Apc->PlayerBark(BarkText, MyUID);

    GetAdventureController()->EndBark.AddUObject(this, &UBarkTask::BarkCompleted);
}

AAdventurePlayerController* UBarkTask::GetAdventureController() const
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    AAdventurePlayerController* AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
    return AdventurePlayerController;
}

void UBarkTask::BarkCompleted(int32 UID)
{
    if (UID == MyUID)
    {
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Task completion message UID: %d - BarkText: %s"),
               UID, *(BarkText.ToString()));
        TaskCompleted.Broadcast();
        SetReadyToDestroy();
    }
    else
    {
        UE_LOG(LogAdventureGame, VeryVerbose, TEXT("Ignoring task completion message UID: %d - BarkText: %s"),
               UID, *(BarkText.ToString()));
    }
}
