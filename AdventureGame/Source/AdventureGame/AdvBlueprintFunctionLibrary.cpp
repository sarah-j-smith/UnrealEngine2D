// (c) 2025 Sarah Smith


#include "AdvBlueprintFunctionLibrary.h"
#include "AdventurePlayerController.h"
#include "ItemList.h"
#include "Kismet/GameplayStatics.h"

AAdventurePlayerController* UAdvBlueprintFunctionLibrary::GetAdventureController(const UObject* WorldContextObject)
{
    if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        APlayerController *PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        return Cast<AAdventurePlayerController>(PlayerController);
    }
    return nullptr;
}

void UAdvBlueprintFunctionLibrary::Bark(const UObject* WorldContextObject, FText BarkText)
{
    if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController(WorldContextObject))
    {
        AdventurePlayerController->PlayerBark(BarkText);
    }
}

void UAdvBlueprintFunctionLibrary::ClearVerb(const UObject* WorldContextObject)
{
    if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController(WorldContextObject))
    {
        AdventurePlayerController->InterruptCurrentAction();
    }
}

int32 UAdvBlueprintFunctionLibrary::PIEInstance(const UObject* WorldContextObject)
{
    // https://blog.jamesbrooks.net/posts/pieinstance-in-unreal/
    const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

    // If we are in the editor, return the PIE instance ID, otherwise return -1 (not in PIE and we won't be using this value anyway)
    if (World->IsEditorWorld())
    {
        // Get world context
        if (const FWorldContext* WorldContext = GEngine->GetWorldContextFromWorld(World))
        {
            return WorldContext->PIEInstance;
        }
    }
    return -1;
}

void UAdvBlueprintFunctionLibrary::AddToInventory(const UObject* WorldContextObject, EItemList ItemToAdd)
{
    if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController(WorldContextObject))
    {
        AdventurePlayerController->OnItemAddToInventory(ItemToAdd);
    }
}

void UAdvBlueprintFunctionLibrary::RemoveFromInventory(const UObject* WorldContextObject, EItemList ItemToRemove)
{
    if (AAdventurePlayerController *AdventurePlayerController = GetAdventureController(WorldContextObject))
    {
        AdventurePlayerController->OnItemRemoveFromInventory(ItemToRemove);
    }
}

FString UAdvBlueprintFunctionLibrary::GetProjectVersion()
{
    FString ProjectVersion;

    GConfig->GetString(
      TEXT("/Script/EngineSettings.GeneralProjectSettings"),
      TEXT("ProjectVersion"),
      ProjectVersion,
      GGameIni
    );

    return ProjectVersion;
}