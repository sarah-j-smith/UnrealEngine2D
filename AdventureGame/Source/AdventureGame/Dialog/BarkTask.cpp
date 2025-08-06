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
    , TimeToBark(0.0f)
    , BarkLocation(nullptr)
    , TextColor(FColor::White)
{
    MyUID = AdvGameUtils::GetUUID();
}

UBarkTask* UBarkTask::DoBarkTask(const UObject* WorldContextObject, const FText BarkText, float BarkTime,
                                 USphereComponent* BarkLocation, FColor TextColor)
{
    UBarkTask* BlueprintNode = NewObject<UBarkTask>();
    BlueprintNode->WorldContextObject = WorldContextObject;
    BlueprintNode->TextColor = TextColor;
    BlueprintNode->BarkLocation = BarkLocation;
    BlueprintNode->TimeToBark = BarkTime;

    UE_LOG(LogAdventureGame, Log, TEXT("UBarkTask created"));

    // Register with the game instance to avoid being garbage collected
    BlueprintNode->RegisterWithGameInstance(WorldContextObject);
    return BlueprintNode;
}

void UBarkTask::Activate()
{
    Super::Activate();

    UE_LOG(LogAdventureGame, Log, TEXT("UBarkTask Activate - %s"), *(BarkText.ToString()));

    auto Apc = GetAdventureController();
    Apc->PlayerBark(BarkText, TextColor, TimeToBark, BarkLocation, MyUID);

    GetAdventureController()->EndBark.AddUObject(this, &UBarkTask::BarkCompleted);
}

AAdventurePlayerController* UBarkTask::GetAdventureController() const
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
    AAdventurePlayerController* AdventurePlayerController = Cast<AAdventurePlayerController>(PlayerController);
    return AdventurePlayerController;
}

void UBarkTask::BarkCompleted(FText Text, int32 UID, bool bSuccess)
{
    if (UID == MyUID)
    {
        UE_LOG(LogAdventureGame, Verbose, TEXT("Task completion message UID: %d - BarkText: %s"),
               UID, *(BarkText.ToString()));
        if (bSuccess)
        {
            TaskCompleted.Broadcast();
        }
        else
        {
            TaskInterrupted.Broadcast();
        }
        SetReadyToDestroy();
    }
    else
    {
        UE_LOG(LogAdventureGame, Verbose, TEXT("Ignoring task completion message UID: %d - BarkText: %s"),
               UID, *(BarkText.ToString()));
    }
}
